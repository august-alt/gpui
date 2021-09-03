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

#include "../model/templatefilter.h"

using namespace model;
using namespace model::registry;

namespace gpui {

enum FilterComboValue {
    FilterComboValue_ANY,
    FilterComboValue_YES,
    FilterComboValue_NO,
};

class TemplateFilterDialogPrivate {
public:
    TemplateFilterDialogPrivate() {
        
    }

    QComboBox *managedCombo;
    QComboBox *configuredCombo;
    QComboBox *commentedCombo;
    QGroupBox *keywordGroupBox;
    QLineEdit *keywordEdit;
    QComboBox *keywordCombo;
    QCheckBox *titleCheck;
    QCheckBox *helpCheck;
    QCheckBox *commentCheck;

    QHash<QWidget *, QVariant> originalState;

    QList<QWidget *> getWidgetList() const;
};

TemplateFilterDialog::TemplateFilterDialog(QWidget *parent)
    : QDialog(parent)
    , d(new TemplateFilterDialogPrivate())
{
    setWindowTitle(tr("Filter Options"));

    auto comboLayout = new QFormLayout();

    auto makeFilterCombo = [&](const QString &label)
    {
        auto combo = new QComboBox();
        combo->addItem(tr("Any"), FilterComboValue_ANY);
        combo->addItem(tr("Yes"), FilterComboValue_YES);
        combo->addItem(tr("No"), FilterComboValue_NO);
        comboLayout->addRow(label, combo);

        return combo;
    };

    d->managedCombo = makeFilterCombo(tr("Managed:"));
    d->configuredCombo = makeFilterCombo(tr("Configured:"));
    d->commentedCombo = makeFilterCombo(tr("Commented:"));

    d->keywordEdit = new QLineEdit();

    d->keywordCombo = new QComboBox();
    d->keywordCombo->addItem(tr("Any"), KeywordFilterType_ANY);
    d->keywordCombo->addItem(tr("Exact"), KeywordFilterType_EXACT);
    d->keywordCombo->addItem(tr("All"), KeywordFilterType_ALL);

    auto keywordEditLayout = new QHBoxLayout();
    keywordEditLayout->addWidget(d->keywordEdit);
    keywordEditLayout->addWidget(d->keywordCombo);

    d->titleCheck = new QCheckBox(tr("Policy setting title"));
    d->helpCheck = new QCheckBox(tr("Help text"));
    d->commentCheck = new QCheckBox(tr("Comment"));

    auto keywordCheckLayout = new QHBoxLayout();
    keywordCheckLayout->addWidget(d->titleCheck);
    keywordCheckLayout->addWidget(d->helpCheck);
    keywordCheckLayout->addWidget(d->commentCheck);

    d->keywordGroupBox = new QGroupBox(tr("Enable Keyword Filter"));
    d->keywordGroupBox->setCheckable(true);
    d->keywordGroupBox->setChecked(false);

    auto keywordFilterLayout = new QFormLayout();
    d->keywordGroupBox->setLayout(keywordFilterLayout);
    keywordFilterLayout->addRow(tr("Filter for word(s):"), keywordEditLayout);
    keywordFilterLayout->addRow(tr("Within:"), keywordCheckLayout);

    auto buttonBox = new QDialogButtonBox();
    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    auto layout = new QVBoxLayout();
    setLayout(layout);
    layout->addLayout(comboLayout);
    layout->addWidget(d->keywordGroupBox);
    layout->addWidget(buttonBox);

    connect(
        buttonBox, &QDialogButtonBox::accepted,
        this, &TemplateFilterDialog::accept);
    connect(
        buttonBox, &QDialogButtonBox::rejected,
        this, &TemplateFilterDialog::reject);
}

TemplateFilterDialog::~TemplateFilterDialog()
{
    delete d;
}

model::TemplateFilter TemplateFilterDialog::getFilter() const {
    model::TemplateFilter out;

    out.keywordEnabled = d->keywordGroupBox->isChecked();
    out.titleEnabled = d->titleCheck->isChecked();
    out.helpEnabled = d->helpCheck->isChecked();
    out.commentEnabled = d->commentCheck->isChecked();
    out.keywordText = d->keywordEdit->text();
    out.keywordType = d->keywordCombo->currentData().value<KeywordFilterType>();
    
    out.configured = [&]()
    {
        const FilterComboValue configuredState = d->configuredCombo->currentData().value<FilterComboValue>();

        switch (configuredState) {
            case FilterComboValue_ANY: return QSet<PolicyStateManager::PolicyState>({
                PolicyStateManager::STATE_NOT_CONFIGURED,
                PolicyStateManager::STATE_ENABLED,
                PolicyStateManager::STATE_DISABLED,
            });
            case FilterComboValue_YES: return QSet<PolicyStateManager::PolicyState>({
                PolicyStateManager::STATE_ENABLED,
                PolicyStateManager::STATE_DISABLED,
            });
            case FilterComboValue_NO: return QSet<PolicyStateManager::PolicyState>({
                PolicyStateManager::STATE_NOT_CONFIGURED,
            });
        }
        return QSet<PolicyStateManager::PolicyState>();
    }();

    // TODO: save filters from managed and comment combo
    // boxes. Not sure what kind of data that will be yet

    return out;
}

// Save state when opening, to restore it later if
// dialog is rejected
void TemplateFilterDialog::open() {
    const QList<QWidget *> widgetList = d->getWidgetList();
    
    for (QWidget *widget : widgetList) {
        QComboBox *combo = qobject_cast<QComboBox*>(widget);
        QCheckBox *check = qobject_cast<QCheckBox*>(widget);
        QGroupBox *groupbox = qobject_cast<QGroupBox*>(widget);
        QLineEdit *lineedit = qobject_cast<QLineEdit*>(widget);

        if (combo != nullptr) {
            d->originalState[widget] = combo->currentIndex();
        } else if (check != nullptr) {
            d->originalState[widget] = check->isChecked();
        } else if (groupbox != nullptr) {
            d->originalState[widget] = groupbox->isChecked();
        } else if (lineedit != nullptr) {
            d->originalState[widget] = lineedit->text();
        }
    }

    QDialog::open();
}

void TemplateFilterDialog::accept() {
    const bool keywordWithinIsValid = [&]() {
        if (d->keywordGroupBox->isChecked()) {
            const QList<bool> keyword_enabled_list = {
                d->titleCheck->isChecked(),
                d->helpCheck->isChecked(),
                d->commentCheck->isChecked(),
            };

            const bool any_keyword_enabled = keyword_enabled_list.contains(true);

            return any_keyword_enabled;
        } else {
            return true;
        }
    }();

    if (keywordWithinIsValid) {
       QDialog::accept();
    } else {
        const QString title = tr("Filter Error");
        const QString text = tr("Please select one or more keyword filter Within options.");
        QMessageBox::warning(this, title, text);
    }
}

// Restore original state
void TemplateFilterDialog::reject() {
    const QList<QWidget *> widgetList = d->getWidgetList();
    
    for (QWidget *widget : widgetList) {
        QComboBox *combo = qobject_cast<QComboBox*>(widget);
        QCheckBox *check = qobject_cast<QCheckBox*>(widget);
        QGroupBox *groupbox = qobject_cast<QGroupBox*>(widget);
        QLineEdit *lineedit = qobject_cast<QLineEdit*>(widget);

        if (combo != nullptr) {
            const int index = d->originalState[widget].value<int>();
            combo->setCurrentIndex(index);
        } else if (check != nullptr) {
            const bool isChecked = d->originalState[widget].value<bool>();
            check->setChecked(isChecked);
        } else if (groupbox != nullptr) {
            const bool isChecked = d->originalState[widget].value<bool>();
            groupbox->setChecked(isChecked);
        } else if (lineedit != nullptr) {
            const QString text = d->originalState[widget].value<QString>();
            lineedit->setText(text);
        }
    }

    QDialog::reject();
}

// NOTE: add any new widgets you add to this list so that
// their state is saved
QList<QWidget *> TemplateFilterDialogPrivate::getWidgetList() const {
    const QList<QWidget *> out = {
        managedCombo,
        configuredCombo,
        commentedCombo,
        keywordGroupBox,
        keywordEdit,
        keywordCombo,
        titleCheck,
        helpCheck,
        commentCheck,
    };

    return out;
}

}

Q_DECLARE_METATYPE(gpui::FilterComboValue)
Q_DECLARE_METATYPE(model::KeywordFilterType)
