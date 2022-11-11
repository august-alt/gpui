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

#include "scriptwidgetcommonslots.h"

#include "addscriptwidget.h"
#include "scriptitem.h"
#include "scriptitemcontainer.h"
#include "scriptsmodel.h"

#include "../../gui/filedialogutils.h"
#include "../../io/genericreader.h"
#include <mvvm/model/modelutils.h>
#include <mvvm/viewmodel/viewitem.h>

#include "../../io/genericwriter.h"
#include "../../io/inifile.h"

#include <QDebug>

namespace scripts_plugin
{
ScriptWidgetCommonSlots::ScriptWidgetCommonSlots(QWidget *parentWidget)
    : parent(parentWidget)
{}

void ScriptWidgetCommonSlots::onUpClicked()
{
    ModelView::Utils::MoveUp(selectedItem->item()->parent());
}

void ScriptWidgetCommonSlots::onDownClicked()
{
    ModelView::Utils::MoveDown(selectedItem->item()->parent());
}

void ScriptWidgetCommonSlots::onAddClicked()
{
    auto newItem = rootItem->insertItem<ScriptItem>({"", 0});

    auto addWidget = new AddScriptWidget(parent);

    addWidget->setDeletingFlag(true);
    addWidget->setWindowTitle("Add script");
    addWidget->setItem(newItem);
    addWidget->setModal(true);

    addWidget->show();
}

void ScriptWidgetCommonSlots::onEditClicked()
{
    auto addWidget = new AddScriptWidget(parent);

    addWidget->setWindowTitle("Edit script");
    addWidget->setItem(selectedItem->item()->parent());
    addWidget->setModal(true);

    addWidget->show();
}

void ScriptWidgetCommonSlots::onDeleteClicked()
{
    if (selectedItem && selectedItem->item()->parent())
    {
        auto parentItem = selectedItem->item()->parent();

        sessionModel->removeItem(parentItem->parent(), parentItem->tagRow());
    }
    else
    {
        qWarning() << "Selected item: " << selectedItem
                   << " Parent: " << selectedItem->item()->parent();
    }
}

void ScriptWidgetCommonSlots::onBrowseClicked()
{
    auto dialog = new gpui::FileDialogUtils;
    QString file = dialog->getOpenFileName();

    if(!file.isEmpty()) {
        loadIniFile(file);
    }

    delete dialog;

}

void ScriptWidgetCommonSlots::onOkClicked() {}

void ScriptWidgetCommonSlots::loadIniFile(QString file)
{
    auto stringValues = std::make_unique<std::string>();

    try
    {
        QFile registryFile(file);
        registryFile.open(QFile::ReadWrite);
        stringValues->resize(registryFile.size(), 0);
        registryFile.read(&stringValues->at(0), registryFile.size());
        registryFile.close();

        auto iss = std::make_unique<std::istringstream>(*stringValues);
        std::string pluginName("ini");

        auto reader  = std::make_unique<io::GenericReader>();
        auto iniFile = reader->load<io::IniFile, io::PolicyFileFormat<io::IniFile>>(*iss,
                                                                                    pluginName);
        if (!iniFile)
        {
            qWarning() << "Unable to load registry file contents.";
            return;
        }

        sessionModel->clear();

        auto sections = iniFile->getAllSections();

        for (const auto &section : sections->keys())
        {
            auto container = sessionModel->insertItem<ScriptItemContainer>();
            container->setProperty(ScriptItemContainer::SECTION_NAME, section);
            container->setDisplayName(section);

            auto group = container->getScripts();

            for (const auto &script_path : sections.get()->value(section).keys())
            {
                auto item = group->insertItem<ScriptItem>(GroupScriptContainerItem::ITEM);

                item->setProperty(ScriptItem::PATH, script_path);
                item->setProperty(ScriptItem::PARAMETER,
                                  sections.get()->value(section).value(script_path));
            }
        }
    }
    catch (std::exception &e)
    {
        qWarning() << "Warning: Unable to read file: " << file.toStdString().c_str()
                   << " description: " << e.what();
    }

}

} // namespace scripts_plugin
