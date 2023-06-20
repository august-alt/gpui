/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd. <org@basealt.ru>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#include "templatefilterdialog.h"
#include "ui_templatefilterdialog.h"

#include "templatefilter.h"

#include "platformmodel.h"

using namespace model;
using namespace model::registry;

namespace gpui
{
enum FilterComboValue
{
    FilterComboValue_ANY,
    FilterComboValue_YES,
    FilterComboValue_NO,
};

class TemplateFilterDialogPrivate
{
public:
    TemplateFilterDialogPrivate() {}

    Ui::TemplateFilterDialog *ui = nullptr;

    QHash<QWidget *, QVariant> originalState{};

    QList<QWidget *> getWidgetList() const;

    TemplateFilterDialogPrivate(const TemplateFilterDialogPrivate &) = delete;            // copy ctor
    TemplateFilterDialogPrivate(TemplateFilterDialogPrivate &&)      = delete;            // move ctor
    TemplateFilterDialogPrivate &operator=(const TemplateFilterDialogPrivate &) = delete; // copy assignment
    TemplateFilterDialogPrivate &operator=(TemplateFilterDialogPrivate &&) = delete;      // move assignment
};

TemplateFilterDialog::TemplateFilterDialog(QWidget *parent)
    : QDialog(parent)
    , d(new TemplateFilterDialogPrivate())
{
    d->ui = new Ui::TemplateFilterDialog();
    d->ui->setupUi(this);
}

TemplateFilterDialog::~TemplateFilterDialog()
{
    delete d->ui;

    delete d;
}

TemplateFilter TemplateFilterDialog::getFilter() const
{
    TemplateFilter out;

    out.keywordEnabled  = d->ui->keywordCheckBox->isChecked();
    out.titleEnabled    = d->ui->titleCheckBox->isChecked();
    out.helpEnabled     = d->ui->helpCheckBox->isChecked();
    out.commentEnabled  = d->ui->commentCheckBox->isChecked();
    out.platformEnabled = d->ui->platformCheckBox->isChecked();
    out.keywordText     = d->ui->keywordLineEdit->text();
    out.keywordType     = static_cast<KeywordFilterType>(d->ui->keywordComboBox->currentIndex());
    out.platformType    = static_cast<PlatformFilterType>(d->ui->platformComboBox->currentIndex());

    out.selectedPlatforms = [&]() {
        QSet<QString> platforms;

        auto sourceModel = d->ui->platformTreeView->model();

        auto current = sourceModel->index(0, 1);

        for (int row = 0; row < sourceModel->rowCount(current); ++row)
        {
            QModelIndex index = sourceModel->index(row, 0, current);

            auto state = index.data(Qt::CheckStateRole).value<Qt::CheckState>();
            if (state == Qt::Checked)
            {
                platforms.insert(index.data().value<QString>());
            }
        }

        return platforms;
    }();

    out.configured = [&]() {
        const FilterComboValue configuredState = static_cast<FilterComboValue>(
            d->ui->configuredComboBox->currentIndex());

        switch (configuredState)
        {
        case FilterComboValue_ANY:
            return QSet<PolicyStateManager::PolicyState>({
                PolicyStateManager::STATE_NOT_CONFIGURED,
                PolicyStateManager::STATE_ENABLED,
                PolicyStateManager::STATE_DISABLED,
            });
        case FilterComboValue_YES:
            return QSet<PolicyStateManager::PolicyState>({
                PolicyStateManager::STATE_ENABLED,
                PolicyStateManager::STATE_DISABLED,
            });
        case FilterComboValue_NO:
            return QSet<PolicyStateManager::PolicyState>({
                PolicyStateManager::STATE_NOT_CONFIGURED,
            });
        }
        return QSet<PolicyStateManager::PolicyState>();
    }();

    // TODO: save filters from managed and comment combo
    // boxes. Not sure what kind of data that will be yet

    return out;
}

void TemplateFilterDialog::setPlatformModel(PlatformModel *platformModel)
{
    if (platformModel)
    {
        d->ui->platformTreeView->setModel(platformModel);
    }
}

// Save state when opening, to restore it later if
// dialog is rejected
void TemplateFilterDialog::open()
{
    const QList<QWidget *> widgetList = d->getWidgetList();

    for (QWidget *widget : widgetList)
    {
        QComboBox *combo    = qobject_cast<QComboBox *>(widget);
        QCheckBox *check    = qobject_cast<QCheckBox *>(widget);
        QGroupBox *groupbox = qobject_cast<QGroupBox *>(widget);
        QLineEdit *lineedit = qobject_cast<QLineEdit *>(widget);

        if (combo != nullptr)
        {
            d->originalState[widget] = combo->currentIndex();
        }
        else if (check != nullptr)
        {
            d->originalState[widget] = check->isChecked();
        }
        else if (groupbox != nullptr)
        {
            d->originalState[widget] = groupbox->isChecked();
        }
        else if (lineedit != nullptr)
        {
            d->originalState[widget] = lineedit->text();
        }
    }

    QDialog::open();
}

void TemplateFilterDialog::accept()
{
    const bool keywordWithinIsValid = [&]() {
        if (d->ui->keywordCheckBox->isChecked())
        {
            const QList<bool> keyword_enabled_list = {
                d->ui->titleCheckBox->isChecked(),
                d->ui->helpCheckBox->isChecked(),
                d->ui->commentCheckBox->isChecked(),
            };

            const bool any_keyword_enabled = keyword_enabled_list.contains(true);

            return any_keyword_enabled;
        }
        else
        {
            return true;
        }
    }();

    if (keywordWithinIsValid)
    {
        QDialog::accept();
    }
    else
    {
        const QString title = tr("Filter Error");
        const QString text  = tr("Please select one or more keyword filter Within options.");
        QMessageBox::warning(this, title, text);
    }
}

// Restore original state
void TemplateFilterDialog::reject()
{
    const QList<QWidget *> widgetList = d->getWidgetList();

    for (QWidget *widget : widgetList)
    {
        QComboBox *combo    = qobject_cast<QComboBox *>(widget);
        QCheckBox *check    = qobject_cast<QCheckBox *>(widget);
        QGroupBox *groupbox = qobject_cast<QGroupBox *>(widget);
        QLineEdit *lineedit = qobject_cast<QLineEdit *>(widget);

        if (combo != nullptr)
        {
            const int index = d->originalState[widget].value<int>();
            combo->setCurrentIndex(index);
        }
        else if (check != nullptr)
        {
            const bool isChecked = d->originalState[widget].value<bool>();
            check->setChecked(isChecked);
        }
        else if (groupbox != nullptr)
        {
            const bool isChecked = d->originalState[widget].value<bool>();
            groupbox->setChecked(isChecked);
        }
        else if (lineedit != nullptr)
        {
            const QString text = d->originalState[widget].value<QString>();
            lineedit->setText(text);
        }
    }

    QDialog::reject();
}

void TemplateFilterDialog::on_selectPushButton_clicked()
{
    auto sourceModel = d->ui->platformTreeView->model();

    auto current = sourceModel->index(0, 1);

    for (int row = 0; row < sourceModel->rowCount(current); ++row)
    {
        QModelIndex index = sourceModel->index(row, 0, current);

        sourceModel->setData(index, Qt::Checked, Qt::CheckStateRole);
    }
}

void TemplateFilterDialog::on_clearPushButton_clicked()
{
    auto sourceModel = d->ui->platformTreeView->model();

    auto current = sourceModel->index(0, 1);

    for (int row = 0; row < sourceModel->rowCount(current); ++row)
    {
        QModelIndex index = sourceModel->index(row, 0, current);

        sourceModel->setData(index, Qt::Unchecked, Qt::CheckStateRole);
    }
}

// NOTE: add any new widgets you add to this list so that
// their state is saved
QList<QWidget *> TemplateFilterDialogPrivate::getWidgetList() const
{
    const QList<QWidget *> out = {
        ui->managedComboBox,
        ui->configuredComboBox,
        ui->commentedComboBox,
        ui->keywordCheckBox,
        ui->keywordLineEdit,
        ui->keywordComboBox,
        ui->titleCheckBox,
        ui->helpCheckBox,
        ui->commentCheckBox,
    };

    return out;
}

} // namespace gpui

Q_DECLARE_METATYPE(gpui::FilterComboValue)
Q_DECLARE_METATYPE(gpui::KeywordFilterType)
