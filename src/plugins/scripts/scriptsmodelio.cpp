#include "scriptsmodelio.h"
#include "../../plugins/storage/smb/smbclient.h"
#include "../../plugins/storage/smb/smbfile.h"
#include "../../io/genericreader.h"
#include "../../io/genericwriter.h"
#include "groupscriptcontaineritem.h"
#include "../../io/inifile.h"
#include "scriptitem.h"
#include "scriptitemcontainer.h"

#include <QDebug>
#include <QFile>

namespace scripts_plugin
{
ScriptsModelIo::ScriptsModelIo() {}

void ScriptsModelIo::loadPolicies(std::string path,
                                  ScriptsModel *userScripts,
                                  ScriptsModel *userPowerScripts,
                                  ScriptsModel *machineScripts,
                                  ScriptsModel *machinePowerScripts)
{
    auto machinePathScripts      = path + "/Machine/scripts.ini";
    auto machinePathPowerScripts = path + "/Machine/pscripts.ini";
    auto userPathScripts         = path + "/User/scripts.ini";
    auto userPathPowerScripts    = path + "/User/pscripts.ini";

    loadIniFile(machinePathScripts, machineScripts);
    loadIniFile(machinePathPowerScripts, machinePowerScripts);
    loadIniFile(userPathScripts, userScripts);
    loadIniFile(userPathPowerScripts, userPowerScripts);
}

void ScriptsModelIo::savePolicies(std::string path,
                                  ScriptsModel *userScripts,
                                  ScriptsModel *userPowerScripts,
                                  ScriptsModel *machineScripts,
                                  ScriptsModel *machinePowerScripts)
{
    auto machinePathScripts      = path + "/Machine/scripts.ini";
    auto machinePathPowerScripts = path + "/Machine/pscripts.ini";
    auto userPathScripts         = path + "/User/scripts.ini";
    auto userPathPowerScripts    = path + "/User/pscripts.ini";

    saveIniFile(machinePathScripts, machineScripts);
    saveIniFile(machinePathPowerScripts, machinePowerScripts);
    saveIniFile(userPathScripts, userScripts);
    saveIniFile(userPathPowerScripts, userPowerScripts);
}

void ScriptsModelIo::loadIniFile(std::string path, ScriptsModel *model)
{
    QString filePath = QString::fromStdString(path);

    auto stringValues = std::make_unique<std::string>();

    try
    {
        if (filePath.startsWith("smb://"))
        {
            gpui::smb::SmbFile smbLocationItemFile(filePath);
            smbLocationItemFile.open(QFile::ReadOnly);
            stringValues->resize(smbLocationItemFile.size(), 0);
            smbLocationItemFile.read(&stringValues->at(0), smbLocationItemFile.size());
            smbLocationItemFile.close();
        }
        else
        {
            QFile registryFile(filePath);
            registryFile.open(QFile::ReadWrite);
            stringValues->resize(registryFile.size(), 0);
            registryFile.read(&stringValues->at(0), registryFile.size());
            registryFile.close();
        }

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

        model->clear();

        auto sections = iniFile->getAllSections();

        for (const auto &section : sections->keys())
        {
            auto container = model->insertItem<ScriptItemContainer>();
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
        qWarning() << "Warning: Unable to read file: " << path.c_str()
                   << " description: " << e.what();
    }
}

void ScriptsModelIo::saveIniFile(std::string path, ScriptsModel *model)
{
    QString filePath = QString::fromStdString(path);

    auto writer = std::make_unique<io::GenericWriter>();

    auto iniFile = std::make_unique<io::IniFile>();

    for (auto section : model->rootItem()->children())
    {
        auto sectionName = section->property<std::string>(ScriptItemContainer::SECTION_NAME);

        auto container = dynamic_cast<scripts_plugin::ScriptItemContainer *>(section);

        if (container)
        {
            for (auto item : container->getScripts()->children())
            {
                auto pathProperty  = item->property<std::string>(ScriptItem::PATH);
                auto paramProperty = item->property<std::string>(ScriptItem::PARAMETER);

                iniFile->addValue(sectionName,
                                  item->property<std::string>(ScriptItem::PATH),
                                  item->property<std::string>(ScriptItem::PARAMETER));
            }
        }
    }

    try
    {
        if (filePath.startsWith("smb://"))
        {
            std::stringstream fileContent;

            writer->save<io::IniFile, io::PolicyFileFormat<io::IniFile>>(fileContent,
                                                                         "ini",
                                                                         iniFile.get());

            gpui::smb::SmbFile smbLocationItemFile(filePath);

            smbLocationItemFile.open(QFile::ReadWrite);
            smbLocationItemFile.write(fileContent.str().c_str(), fileContent.str().size());
            smbLocationItemFile.close();
        }
        else
        {
            writer->save<io::IniFile, io::PolicyFileFormat<io::IniFile>>(path, iniFile.get());
        }
    }
    catch (std::exception &e)
    {
        qWarning() << "Warning: Unable to write file: " << filePath.toStdString().c_str()
                   << " description: " << e.what();
    }
}

} // namespace scripts_plugin
