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

using namespace model::registry;

namespace gpui {

enum StateComboItem {
    ANY,
    CONFIGURED,
    NOT_CONFIGURED,
};

class TemplateFilterDialogPrivate {
public:
    TemplateFilterDialogPrivate() {
        
    }

    QComboBox *stateFilterEdit;
    QGroupBox *titleFilterGroupBox;
    QLineEdit *titleFilterEdit;

};

TemplateFilterDialog::TemplateFilterDialog(QWidget *parent)
    : QDialog(parent)
    , d(new TemplateFilterDialogPrivate())
{
    setWindowTitle(tr("Filter Options"));

    d->stateFilterEdit = new QComboBox();
    d->stateFilterEdit->addItem(tr("Any"), StateComboItem::ANY);
    d->stateFilterEdit->addItem(tr("Configured"), StateComboItem::CONFIGURED);
    d->stateFilterEdit->addItem(tr("Not configured"), StateComboItem::NOT_CONFIGURED);

    auto stateFilterLayout = new QFormLayout();
    stateFilterLayout->addRow(tr("Configured:"), d->stateFilterEdit);

    d->titleFilterEdit = new QLineEdit();

    d->titleFilterGroupBox = new QGroupBox(tr("Enable title filter"));
    d->titleFilterGroupBox->setCheckable(true);
    d->titleFilterGroupBox->setChecked(false);

    auto titleFilterLayout = new QFormLayout();
    titleFilterLayout->addRow(tr("Title:"), d->titleFilterEdit);
    d->titleFilterGroupBox->setLayout(titleFilterLayout);

    auto buttonBox = new QDialogButtonBox();
    buttonBox->addButton(QDialogButtonBox::Ok);

    auto layout = new QVBoxLayout();
    setLayout(layout);
    layout->addLayout(stateFilterLayout);
    layout->addWidget(d->titleFilterGroupBox);
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

    out.titleFilterEnabled = d->titleFilterGroupBox->isChecked();
    out.titleFilter = d->titleFilterEdit->text();

    const StateComboItem state_item = d->stateFilterEdit->currentData().value<StateComboItem>();
    
    out.stateFilterEnabled = [&]()
    {
        switch (state_item) {
            case StateComboItem::ANY: return false;
            case StateComboItem::CONFIGURED: return true;
            case StateComboItem::NOT_CONFIGURED: return true;
        }
        return false;
    }();

    out.stateFilter = [&]()
    {
        switch (state_item) {
            case StateComboItem::ANY: return QSet<PolicyStateManager::PolicyState>({
                PolicyStateManager::STATE_NOT_CONFIGURED,
                PolicyStateManager::STATE_ENABLED,
                PolicyStateManager::STATE_DISABLED,
            });
            case StateComboItem::CONFIGURED: return QSet<PolicyStateManager::PolicyState>({
                PolicyStateManager::STATE_ENABLED,
                PolicyStateManager::STATE_DISABLED,
            });
            case StateComboItem::NOT_CONFIGURED: return QSet<PolicyStateManager::PolicyState>({
                PolicyStateManager::STATE_NOT_CONFIGURED,
            });
        }
        return QSet<PolicyStateManager::PolicyState>();
    }();

    return out;
}

}

Q_DECLARE_METATYPE(gpui::StateComboItem)
