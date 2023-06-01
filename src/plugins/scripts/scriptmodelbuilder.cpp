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

#include <QRegularExpression>

namespace scripts_plugin
{
ScriptModelBuilder::ScriptModelBuilder() {}

void scripts_plugin::ScriptModelBuilder::makeSectionIfNotFound(const std::string &sectionName,
                                                               const std::string &file_path,
                                                               std::shared_ptr<io::IniFile::sections> sections,
                                                               ScriptsModel *model)
{
    auto found = sections->find(sectionName);
    if (found == sections->end())
    {
        auto container = model->insertItem<ScriptItemContainer>();
        container->setProperty(ScriptItemContainer::SECTION_NAME, sectionName);
        container->setProperty(ScriptItemContainer::INI_FILE_PATH, file_path);
    }
}

void ScriptModelBuilder::iniToModel(ScriptsModel *model, io::IniFile *iniFile, std::string &file_path)
{
    auto sections = iniFile->getAllSections();

    model->clear();

    static const auto reg = QRegularExpression("^\\d+");

    for (const auto &section : sections->keys())
    {
        auto container = model->insertItem<ScriptItemContainer>();
        container->setProperty(ScriptItemContainer::SECTION_NAME, section);
        container->setProperty(ScriptItemContainer::INI_FILE_PATH, file_path);
        auto group = container->getScripts();

        std::string iniCommandPath;
        std::string iniCommandParam;

        for (const auto &path : sections->value(section).keys())
        {
            // Standard regular expressions fail build on i586.
            // So they are replaced with QRegularExpression.
            std::string value       = QString::fromStdString(path).replace(reg, "").toStdString();
            std::string secondValue = QString::fromStdString(sections.get()->value(section).value(path)).toStdString();

            if (value.compare("CmdLine") == 0)
            {
                iniCommandPath = secondValue;
                continue;
            }

            if (value.compare("Parameters") == 0)
            {
                iniCommandParam = secondValue;

                auto item = group->insertItem<ScriptItem>(GroupScriptContainerItem::ITEM);

                item->setProperty(ScriptItem::propertyToString(ScriptItem::PropertyType::PATH), iniCommandPath);
                item->setProperty(ScriptItem::propertyToString(ScriptItem::PropertyType::PARAMETER), iniCommandParam);
            }
        }
    }

    makeSectionIfNotFound("Logon", file_path, sections, model);
    makeSectionIfNotFound("Logoff", file_path, sections, model);
    makeSectionIfNotFound("Startup", file_path, sections, model);
    makeSectionIfNotFound("Shutdown", file_path, sections, model);
}

std::unique_ptr<io::IniFile> ScriptModelBuilder::modelToIni(ScriptsModel *model)
{
    auto iniFile = std::make_unique<io::IniFile>();

    for (auto section : model->rootItem()->children())
    {
        auto sectionName = section->property<std::string>(ScriptItemContainer::SECTION_NAME);

        auto container = dynamic_cast<ScriptItemContainer *>(section);

        if (container)
        {
            int numberOfCommand = 0;
            for (auto item : container->getScripts()->children())
            {
                std::string cmdLine   = std::to_string(numberOfCommand) + "CmdLine";
                std::string paramLine = std::to_string(numberOfCommand) + "Parameters";

                auto path  = item->property<std::string>(ScriptItem::propertyToString(ScriptItem::PropertyType::PATH));
                auto param = item->property<std::string>(
                    ScriptItem::propertyToString(ScriptItem::PropertyType::PARAMETER));

                iniFile->addValue(sectionName, cmdLine, path);

                iniFile->addValue(sectionName, paramLine, param);

                numberOfCommand++;
            }
        }
    }

    return iniFile;
}

} // namespace scripts_plugin
