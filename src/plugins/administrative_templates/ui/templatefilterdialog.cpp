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

#include <algorithm>
#include <utility>

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

    d->ui->commentCheckBox->setVisible(false);
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

    const auto sourceModel = d->ui->platformTreeView->model();

    std::function<void(const QModelIndex &)> addPlatforms = [&](const QModelIndex &index) {
        for (int row = 0; row < sourceModel->rowCount(index); ++row)
        {
            const QModelIndex child = sourceModel->index(row, 0, index);

            const bool isLeaf = (sourceModel->rowCount(child) == 0);
            const auto state  = child.data(Qt::CheckStateRole).value<Qt::CheckState>();
            if (isLeaf && state == Qt::Checked)
            {
                auto sort_key = child.data(PLATFORM_ROLE_SORT).value<QString>();
                auto display_name = child.data().value<QString>();
                out.selectedPlatforms.insert(std::make_pair(sort_key, display_name));
            }

            addPlatforms(child);
        }
    };
    addPlatforms(sourceModel->index(0, 1));

    switch (d->ui->configuredComboBox->currentIndex())
    {
    // NOTE: cases order is dictated by FilterComboValue declaration
    case FilterComboValue_ANY:
        out.configured = QSet<PolicyStateManager::PolicyState>({
            PolicyStateManager::STATE_NOT_CONFIGURED,
            PolicyStateManager::STATE_ENABLED,
            PolicyStateManager::STATE_DISABLED,
        });
        break;
    case FilterComboValue_YES:
        out.configured = QSet<PolicyStateManager::PolicyState>({
            PolicyStateManager::STATE_ENABLED,
            PolicyStateManager::STATE_DISABLED,
        });
        break;
    case FilterComboValue_NO:
        out.configured = QSet<PolicyStateManager::PolicyState>({
            PolicyStateManager::STATE_NOT_CONFIGURED,
        });
        break;
    default:
        out.configured = QSet<PolicyStateManager::PolicyState>();
    }

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
    bool keywordWithinIsValid = true;
    if (d->ui->keywordCheckBox->isChecked())
    {
        const QList<bool> keyword_enabled_list = {
            d->ui->titleCheckBox->isChecked(),
            d->ui->helpCheckBox->isChecked(),
            d->ui->commentCheckBox->isChecked(),
        };
        keywordWithinIsValid = keyword_enabled_list.contains(true);
    }

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

void TemplateFilterDialog::setAllCheckState(Qt::CheckState state)
{
    auto sourceModel = d->ui->platformTreeView->model();
    auto current     = sourceModel->index(0, 1);

    for (int row = 0; row < sourceModel->rowCount(current); ++row)
    {
        QModelIndex index = sourceModel->index(row, 0, current);
        sourceModel->setData(index, state, Qt::CheckStateRole);
    }
}

void TemplateFilterDialog::on_selectPushButton_clicked()
{
    setAllCheckState(Qt::Checked);
}

void TemplateFilterDialog::on_clearPushButton_clicked()
{
    setAllCheckState(Qt::Unchecked);
}

// NOTE: add any new widgets you add to this list so that
// their state is saved
QList<QWidget *> TemplateFilterDialogPrivate::getWidgetList() const
{
    const QList<QWidget *> out = {
        ui->configuredComboBox,
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
