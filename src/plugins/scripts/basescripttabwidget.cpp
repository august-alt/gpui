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

#include "basescripttabwidget.h"
#include "addscriptwidget.h"
#include "scriptitem.h"

#include "../../gui/filedialogutils.h"

#include <mvvm/model/modelutils.h>
#include <mvvm/viewmodel/viewitem.h>
#include <QDebug>
#include <QDesktopServices>

namespace scripts_plugin
{
BaseScriptTabWidget::BaseScriptTabWidget(QWidget *p)
    : parent(p)
{}

void BaseScriptTabWidget::onUpClicked()
{
    auto *item = this->selectedItem;
    if (item != nullptr)
    {
        ModelView::Utils::MoveUp(this->selectedItem->item()->parent());
    }
}

void BaseScriptTabWidget::onDownClicked()
{
    auto *item = this->selectedItem;
    if (item != nullptr)
    {
        ModelView::Utils::MoveDown(this->selectedItem->item()->parent());
    }
}

void BaseScriptTabWidget::onAddClicked()
{
    auto root = findRootItem();

    if (root == nullptr)
    {
        return;
    }

    auto newItem = this->rootItem->insertItem<ScriptItem>({"", 0});

    auto addWidget = new AddScriptWidget(parent);

    addWidget->setDeletingFlag(true);
    addWidget->setWindowTitle(QObject::tr("Add script"));
    addWidget->setItem(newItem);
    addWidget->setModal(true);

    addWidget->show();
}

void BaseScriptTabWidget::onEditClicked()
{
    auto *item = this->selectedItem;
    if (item != nullptr)
    {
        auto addWidget = new AddScriptWidget(parent);

        addWidget->setWindowTitle(QObject::tr("Edit script"));

        addWidget->setItem(this->selectedItem->item()->parent());
        addWidget->setModal(true);

        addWidget->show();
    }
}

void BaseScriptTabWidget::onDeleteClicked()
{
    auto *item = this->selectedItem;
    if (item != nullptr)
    {
        if (this->selectedItem && this->selectedItem->item()->parent())
        {
            auto parentItem = this->selectedItem->item()->parent();

            this->sessionModel->removeItem(parentItem->parent(), parentItem->tagRow());

            this->selectedItem = nullptr;
        }
        else
        {
            qWarning() << "Selected item: " << this->selectedItem
                       << " Parent: " << this->selectedItem->item()->parent();
        }
    }
}

void BaseScriptTabWidget::onBrowseClicked()
{
    auto path = scriptsItemContainer->property<std::string>(ScriptItemContainer::INI_FILE_PATH);

    QString dirName = QFileInfo(QString::fromStdString(path)).absolutePath();

    QDesktopServices::openUrl(QUrl(dirName, QUrl::TolerantMode));
}

ScriptItemContainer *BaseScriptTabWidget::findRootItem()
{
    std::string machineSectionName = "Shutdown";
    std::string userSectionName    = "Logoff";

    if (this->isStartUpScripts)
    {
        machineSectionName = "Startup";
        userSectionName    = "Logon";
    }

    if (!this->sessionModel)
    {
        qCritical() << "Section model is NULL!";
        return nullptr;
    }

    auto containers = this->sessionModel->topItems();

    for (size_t i = 0; i < containers.size(); i++)
    {
        auto itemContainer = containers[i];

        auto section = dynamic_cast<ScriptItemContainer *>(itemContainer);

        if (section)
        {
            if (machineSectionName.compare(section->property<std::string>(ScriptItemContainer::SECTION_NAME)) == 0)
            {
                return section;
            }

            if (userSectionName.compare(section->property<std::string>(ScriptItemContainer::SECTION_NAME)) == 0)
            {
                return section;
            }
        }
    }

    qWarning() << "Section:" << userSectionName.c_str() << " or " << machineSectionName.c_str() << " not found!!";

    return nullptr;
}

BaseScriptTabWidget::~BaseScriptTabWidget() {}

} // namespace scripts_plugin
