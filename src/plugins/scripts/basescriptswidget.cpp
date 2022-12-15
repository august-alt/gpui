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

#include "basescriptswidget.h"
#include "addscriptwidget.h"
#include "scriptitem.h"

#include "../../gui/filedialogutils.h"

#include <mvvm/model/modelutils.h>
#include <mvvm/viewmodel/viewitem.h>
#include <QDebug>

namespace scripts_plugin
{
BaseScriptWidget::BaseScriptWidget(QWidget *p)
    : parent(p)
{}

void BaseScriptWidget::onUpClicked()
{
    ModelView::Utils::MoveUp(this->selectedItem->item()->parent());
}

void BaseScriptWidget::onDownClicked()
{
    ModelView::Utils::MoveDown(this->selectedItem->item()->parent());
}

void BaseScriptWidget::onAddClicked(bool isScripts)
{
    auto root = findRootItem(isScripts);

    if (root == nullptr)
    {
        return;
    }

    auto newItem = this->rootItem->insertItem<ScriptItem>({"", 0});

    auto addWidget = new AddScriptWidget(parent);

    addWidget->setDeletingFlag(true);
    addWidget->setWindowTitle("Add script");
    addWidget->setItem(newItem);
    addWidget->setModal(true);

    addWidget->show();
}

void BaseScriptWidget::onEditClicked()
{
    auto *item = this->selectedItem;
    if (item != nullptr)
    {
        auto addWidget = new AddScriptWidget(parent);

        addWidget->setWindowTitle("Edit script");

        addWidget->setItem(this->selectedItem->item()->parent());
        addWidget->setModal(true);

        addWidget->show();
    }
}

void BaseScriptWidget::onDeleteClicked()
{
    if (this->selectedItem && this->selectedItem->item()->parent())
    {
        auto parentItem = this->selectedItem->item()->parent();

        this->sessionModel->removeItem(parentItem->parent(), parentItem->tagRow());
    }
    else
    {
        qWarning() << "Selected item: " << this->selectedItem
                   << " Parent: " << this->selectedItem->item()->parent();
    }
}

void BaseScriptWidget::onBrowseClicked()
{
    auto dialog  = new gpui::FileDialogUtils;
    QString file = dialog->getOpenFileName();

    if (!file.isEmpty())
    {}

    delete dialog;
}

ScriptItemContainer *BaseScriptWidget::findRootItem(bool isScripts)
{
    std::string sectionName = "Shutdown";

    if (isScripts)
    {
        if (this->isStartUpScripts)
        {
            sectionName = "Logon";
        }
        else
        {
            sectionName = "Logoff";
        }
    }
    else
    {
        if (this->isStartUpScripts)
        {
            sectionName = "Startup";
        }
    }

    auto containers = this->sessionModel->topItems();

    for (size_t i = 0; i < containers.size(); i++)
    {
        auto itemContainer = containers[i];

        auto section = dynamic_cast<ScriptItemContainer *>(itemContainer);

        if (section)
        {
            if (sectionName.compare(
                    section->property<std::string>(ScriptItemContainer::SECTION_NAME))
                == 0)
            {
                return section;
            }
        }
    }

    qWarning() << "Section:" << sectionName.c_str() << " not found!!";

    return nullptr;
}

BaseScriptWidget::~BaseScriptWidget() {}

} // namespace scripts_plugin
