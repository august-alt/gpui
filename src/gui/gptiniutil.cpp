/***********************************************************************************************************************
**
** Copyright (C) 2024 BaseALT Ltd. <org@basealt.ru>
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

#include "gptiniutil.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include <stdexcept>
#include <memory>

#include "../io/genericreader.h"
#include "../io/registryfile.h"
#include "../io/registryfileformat.h"

#include "../io/policydefinitionsfile.h"
#include "../io/policyfileformat.h"

#include "../io/inifile.h"
#include "../plugins/ini/iniformat.h"

#include "../plugins/storage/smb/smbfile.h"

namespace gpui
{

bool GptIniUtils::onIniFileOpen(const QString &path, QString &policyName, int &machineVersion, int &userVersion)
{
    qWarning() << "Path recieved: " << path;

    auto stringvalues = std::make_unique<std::string>();

    try {
        if (path.startsWith("smb://"))
        {
            gpui::smb::SmbFile smbLocationItemFile(path);
            smbLocationItemFile.open(QFile::ReadOnly);
            stringvalues->resize(smbLocationItemFile.size(), 0);
            smbLocationItemFile.read(&stringvalues->at(0), smbLocationItemFile.size());
            smbLocationItemFile.close();
        }
        else
        {
            QFile registryFile(path);
            registryFile.open(QFile::ReadWrite);
            stringvalues->resize(registryFile.size(), 0);
            registryFile.read(&stringvalues->at(0), registryFile.size());
            registryFile.close();
        }

        auto iss = std::make_unique<std::istringstream>(*stringvalues);
        std::string pluginName("ini");

        auto reader = std::make_unique<io::GenericReader>();
        auto iniFile = reader->load<io::IniFile, io::PolicyFileFormat<io::IniFile> >(*iss, pluginName);
        if (!iniFile)
        {
            qWarning() << "Unable to load registry file contents.";
            return false;
        }

        auto sections = iniFile->getAllSections();

        if (sections->find("General") != sections->end())
        {
            auto& generalValues = (*sections)["General"];

            auto displayName = generalValues.find("displayName");

            if (displayName != generalValues.end())
            {
                policyName = QString::fromStdString(*displayName);
            }

            auto version = generalValues.find("Version");
            if (version != generalValues.end())
            {
                int intVersion = QString::fromStdString(*version).toInt();
                machineVersion = intVersion & 0x0000FFFF;
                userVersion = (intVersion & 0xFFFF0000) >> 16;
            }
        }

        return true;
    }
    catch (std::exception& e)
    {
        QMessageBox messageBox(QMessageBox::Critical,
                    QObject::tr("Error"),
                    QObject::tr("Error reading file:") + "\n" + qPrintable(path),
                    QMessageBox::Ok,
                    nullptr);
        messageBox.exec();
    }

    return false;
}

bool GptIniUtils::onIniFileSave(const QString &path, const QString &policyName, const int machineVersion, const int userVersion)
{
    std::unique_ptr<io::IniFile> fileData = std::make_unique<io::IniFile>();
    fileData->addValue("General", "DisplayName", policyName.toStdString());
    int version = 0;
    fileData->addValue("General", "Version", std::to_string(version));

    QString pluginName = path;
    pluginName         = pluginName.mid(pluginName.lastIndexOf('.') + 1);

    gpui::IniFormat *format = gpui::PluginStorage::instance()->createPluginClass<gpui::IniFormat>(pluginName);

    if (!format)
    {
        qWarning() << "Format supporting: " << pluginName << " not found.";

        return false;
    }

    auto oss = std::make_unique<std::ostringstream>();

    if (!format->write(*oss, fileData.get()))
    {
        qWarning() << path << " " << format->getErrorString().c_str();
    }

    oss->flush();

    qWarning() << "Current string values." << oss->str().c_str();

    bool ifShowError = false;

    auto showMessageFunction = [&path]() {
        QMessageBox messageBox(QMessageBox::Critical,
                               QObject::tr("Error"),
                               QObject::tr("Error writing file:") + "\n" + qPrintable(path),
                               QMessageBox::Ok);
        messageBox.exec();
    };

    try
    {
        if (path.startsWith("smb://"))
        {
            gpui::smb::SmbFile smbLocationItemFile(path);
            ifShowError = smbLocationItemFile.open(QFile::WriteOnly | QFile::Truncate);
            if (!ifShowError)
            {
                ifShowError = smbLocationItemFile.open(QFile::NewOnly | QFile::WriteOnly);
            }
            if (ifShowError && oss->str().size() > 0)
            {
                smbLocationItemFile.write(&oss->str().at(0), oss->str().size());
            }
            smbLocationItemFile.close();
        }
        else
        {
            QFile registryFile(path);
            ifShowError = registryFile.open(QFile::WriteOnly | QFile::Truncate);
            if (!ifShowError)
            {
                ifShowError = registryFile.open(QFile::NewOnly | QFile::WriteOnly);
            }
            if (ifShowError && registryFile.isWritable() && oss->str().size() > 0)
            {
                registryFile.write(&oss->str().at(0), oss->str().size());
            }
            registryFile.close();
        }
    }
    catch (std::exception &e)
    {
        ifShowError = true;
        showMessageFunction();

        return false;
    }

    if (!ifShowError)
    {
        showMessageFunction();
    }

    delete format;

    return true;
}

}
