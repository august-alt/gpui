#include "scriptsmodelio.h"
#include "../../io/genericreader.h"
#include "../../io/genericwriter.h"
#include "../../io/inifile.h"
#include "../../plugins/storage/smb/smbclient.h"
#include "../../plugins/storage/smb/smbfile.h"
#include "../../plugins/storage/smb/smbdirectory.h"
#include "groupscriptcontaineritem.h"
#include "scriptitem.h"
#include "scriptitemcontainer.h"
#include "scriptmodelbuilder.h"

#include "../../core/logger/log.h"
#include <QFile>

namespace scripts_plugin
{
ScriptsModelIo::ScriptsModelIo() {}

std::string ScriptsModelIo::correctPath(const std::string &path)
{
    std::string newPath = path;

    if (*newPath.rbegin() != '/' && *newPath.rbegin() != '\\')
    {
        newPath.append("/");
    }

    return newPath;
}

void ScriptsModelIo::loadPolicies(const std::string &path,
                                  ScriptsModel *userScripts,
                                  ScriptsModel *userPowerScripts,
                                  ScriptsModel *machineScripts,
                                  ScriptsModel *machinePowerScripts)
{
    if (!path.empty())
    {
        std::string newPath = correctPath(path);

        auto machinePathScripts      = newPath + "Machine/Scripts/scripts.ini";
        auto machinePathPowerScripts = newPath + "Machine/Scripts/psscripts.ini";
        auto userPathScripts         = newPath + "User/Scripts/scripts.ini";
        auto userPathPowerScripts    = newPath + "User/Scripts/psscripts.ini";

        loadIniFile(machinePathScripts, machineScripts);
        loadIniFile(machinePathPowerScripts, machinePowerScripts);
        loadIniFile(userPathScripts, userScripts);
        loadIniFile(userPathPowerScripts, userPowerScripts);
    }
}

void ScriptsModelIo::savePolicies(const std::string &path,
                                  ScriptsModel *userScripts,
                                  ScriptsModel *userPowerScripts,
                                  ScriptsModel *machineScripts,
                                  ScriptsModel *machinePowerScripts)
{
    if (!path.empty())
    {
        std::string newPath = correctPath(path);

        createDirectory(newPath + "Machine/Scripts/");
        createDirectory(newPath + "User/Scripts/");

        createDirectory(newPath + "Machine/Scripts/Startup");
        createDirectory(newPath + "Machine/Scripts/Shutdown");
        createDirectory(newPath + "User/Scripts/Logon");
        createDirectory(newPath + "User/Scripts/Logoff");

        auto machinePathScripts      = newPath + "Machine/Scripts/scripts.ini";
        auto machinePathPowerScripts = newPath + "Machine/Scripts/psscripts.ini";
        auto userPathScripts         = newPath + "User/Scripts/scripts.ini";
        auto userPathPowerScripts    = newPath + "User/Scripts/psscripts.ini";

        saveIniFile(machinePathScripts, machineScripts);
        saveIniFile(machinePathPowerScripts, machinePowerScripts);
        saveIniFile(userPathScripts, userScripts);
        saveIniFile(userPathPowerScripts, userPowerScripts);
    }
}

void ScriptsModelIo::loadIniFile(std::string &path, ScriptsModel *model)
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
            registryFile.open(QFile::ReadOnly);
            stringValues->resize(registryFile.size(), 0);
            registryFile.read(&stringValues->at(0), registryFile.size());
            registryFile.close();
        }

        auto iss = std::make_unique<std::istringstream>(*stringValues);
        std::string pluginName("ini");

        auto reader  = std::make_unique<io::GenericReader>();
        auto iniFile = reader->load<io::IniFile, io::PolicyFileFormat<io::IniFile>>(*iss, pluginName);
        if (!iniFile)
        {
            GPUI_WARNING_STREAM << "Unable to load registry file contents.";
            return;
        }

        ScriptModelBuilder builder;

        builder.iniToModel(model, iniFile.get(), path);
    }
    catch (std::exception &e)
    {
        GPUI_WARNING_STREAM << "Warning: Unable to read file: " << path.c_str() << " description: " << e.what();
    }
}

void ScriptsModelIo::saveIniFile(std::string &path, ScriptsModel *model)
{
    QString filePath = QString::fromStdString(path);

    auto writer = std::make_unique<io::GenericWriter>();

    ScriptModelBuilder builder;

    auto iniFile = builder.modelToIni(model);

    try
    {
        if (filePath.startsWith("smb://"))
        {
            std::stringstream fileContent;

            writer->save<io::IniFile, io::PolicyFileFormat<io::IniFile>>(fileContent, "ini", iniFile.get());

            gpui::smb::SmbFile smbLocationItemFile(filePath);

            GPUI_WARNING_STREAM << "Script file path: " << filePath;

            bool openResult = smbLocationItemFile.open(QFile::WriteOnly | QFile::Truncate);
            if (!openResult)
            {
                GPUI_WARNING_STREAM << "Unable to open file for writing trying to create new file.";
                openResult = smbLocationItemFile.open(QFile::NewOnly | QFile::WriteOnly);
            }
            if (openResult && fileContent.str().size() > 0)
            {
                smbLocationItemFile.write(&fileContent.str().at(0), fileContent.str().size());
            }
            else
            {
                GPUI_WARNING_STREAM << "Unable to open new file!";
            }
            smbLocationItemFile.close();
        }
        else
        {
            writer.get()->save<io::IniFile, io::PolicyFileFormat<io::IniFile>>(path, iniFile.get());
        }
    }
    catch (std::exception &e)
    {
        GPUI_WARNING_STREAM << "Warning: Unable to write file: " << filePath.toStdString().c_str()
                   << " description: " << e.what();
    }
}

void ScriptsModelIo::createDirectory(const std::string &directoryName)
{
    const QString path = QString::fromStdString(directoryName);

    if (path.startsWith("smb://"))
    {
        gpui::smb::SmbDirectory dir(path);

        if (!dir.exists())
        {
            dir.mkdir(path);
        }
    }
    else
    {
        QDir dir(path);

        if (!dir.exists())
        {
            dir.mkdir(path);
        }
    }
}

} // namespace scripts_plugin
