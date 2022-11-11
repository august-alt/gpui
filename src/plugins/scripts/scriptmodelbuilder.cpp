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

#include "scriptmodelbuilder.h"
#include "groupscriptcontaineritem.h"
#include "scriptitem.h"
#include "scriptitemcontainer.h"

#include "../../io/genericwriter.h"

namespace scripts_plugin
{
ScriptModelBuilder::ScriptModelBuilder() {}

std::unique_ptr<ScriptsModel> ScriptModelBuilder::iniToModel(std::unique_ptr<io::IniFile> iniFile)
{
    auto sections = iniFile->getAllSections();

    auto resultModel = std::make_unique<ScriptsModel>();
    resultModel->clear();

    for (const auto &section : sections->keys())
    {
        auto container = resultModel->insertItem<ScriptItemContainer>();
        container->setProperty(ScriptItemContainer::SECTION_NAME, section);
        auto group = container->getScripts();

        for (const auto &path : sections.get()->value(section).keys())
        {
            auto item = group->insertItem<ScriptItem>(GroupScriptContainerItem::ITEM);

            item->setProperty(ScriptItem::PATH, path);
            item->setProperty(ScriptItem::PARAMETER, sections.get()->value(section).value(path));
        }
    }

    return resultModel;
}

std::unique_ptr<io::IniFile> ScriptModelBuilder::modelToIni(std::unique_ptr<ScriptsModel> model)
{
    auto iniFile = std::make_unique<io::IniFile>();

    for (auto section : model->rootItem()->children())
    {
        auto sectionName = section->property<std::string>(ScriptItemContainer::SECTION_NAME);

        for (auto item : section->children())
        {
            auto path  = item->property<std::string>(ScriptItem::PATH);
            auto param = item->property<std::string>(ScriptItem::PARAMETER);

            iniFile->addValue(sectionName,
                              item->property<std::string>(ScriptItem::PATH),
                              item->property<std::string>(ScriptItem::PARAMETER));
        }
    }

    return iniFile;
}

} // namespace scripts_plugin
