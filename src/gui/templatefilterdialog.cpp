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

    QComboBox *managedFilterCombo;
    QComboBox *configuredCombo;
    QComboBox *commentedFilterCombo;
    QGroupBox *keywordFilterGroupBox;
    QLineEdit *keywordFilterEdit;
    QComboBox *keywordFilterCombo;
    QCheckBox *keywordTitleCheck;
    QCheckBox *keywordHelpCheck;
    QCheckBox *keywordCommentCheck;

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

    d->managedFilterCombo = makeFilterCombo(tr("Managed:"));
    d->configuredCombo = makeFilterCombo(tr("Configured:"));
    d->commentedFilterCombo = makeFilterCombo(tr("Commented:"));

    d->keywordFilterEdit = new QLineEdit();

    d->keywordFilterCombo = new QComboBox();
    d->keywordFilterCombo->addItem(tr("Any"), KeywordFilterType_ANY);
    d->keywordFilterCombo->addItem(tr("Exact"), KeywordFilterType_EXACT);
    d->keywordFilterCombo->addItem(tr("All"), KeywordFilterType_ALL);

    auto keywordEditLayout = new QHBoxLayout();
    keywordEditLayout->addWidget(d->keywordFilterEdit);
    keywordEditLayout->addWidget(d->keywordFilterCombo);

    d->keywordTitleCheck = new QCheckBox(tr("Policy setting title"));
    d->keywordHelpCheck = new QCheckBox(tr("Help text"));
    d->keywordCommentCheck = new QCheckBox(tr("Comment"));

    auto keywordCheckLayout = new QHBoxLayout();
    keywordCheckLayout->addWidget(d->keywordTitleCheck);
    keywordCheckLayout->addWidget(d->keywordHelpCheck);
    keywordCheckLayout->addWidget(d->keywordCommentCheck);

    d->keywordFilterGroupBox = new QGroupBox(tr("Enable Keyword Filter"));
    d->keywordFilterGroupBox->setCheckable(true);
    d->keywordFilterGroupBox->setChecked(false);

    auto keywordFilterLayout = new QFormLayout();
    d->keywordFilterGroupBox->setLayout(keywordFilterLayout);
    keywordFilterLayout->addRow(tr("Filter for word(s):"), keywordEditLayout);
    keywordFilterLayout->addRow(tr("Within:"), keywordCheckLayout);

    auto buttonBox = new QDialogButtonBox();
    buttonBox->addButton(QDialogButtonBox::Ok);

    auto layout = new QVBoxLayout();
    setLayout(layout);
    layout->addLayout(comboLayout);
    layout->addWidget(d->keywordFilterGroupBox);
    layout->addWidget(buttonBox);

    connect(
        buttonBox, &QDialogButtonBox::accepted,
        this, &QDialog::accept);
}

TemplateFilterDialog::~TemplateFilterDialog()
{
    delete d;
}

model::TemplateFilter TemplateFilterDialog::getFilter() const {
    model::TemplateFilter out;

    out.keywordEnabled = d->keywordFilterGroupBox->isChecked();
    out.titleEnabled = d->keywordTitleCheck->isChecked();
    out.helpEnabled = d->keywordHelpCheck->isChecked();
    out.commentEnabled = d->keywordCommentCheck->isChecked();
    out.keywordText = d->keywordFilterEdit->text();
    out.keywordType = d->keywordFilterCombo->currentData().value<KeywordFilterType>();
    
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

void TemplateFilterDialog::accept() {
    const bool keywordWithinIsValid = [&]() {
        if (d->keywordFilterGroupBox->isChecked()) {
            const QList<bool> keyword_enabled_list = {
                d->keywordTitleCheck->isChecked(),
                d->keywordHelpCheck->isChecked(),
                d->keywordCommentCheck->isChecked(),
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

}

Q_DECLARE_METATYPE(gpui::FilterComboValue)
Q_DECLARE_METATYPE(model::KeywordFilterType)
