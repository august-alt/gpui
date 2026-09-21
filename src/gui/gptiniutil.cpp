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

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

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

namespace
{

// The canonical name of the file in a GPO is GPT.INI. Depending on the file system of the
// domain controller and on the case sensitivity settings of the share the very same file may
// be visible as gpt.ini, so every spelling that is known to occur is probed.
const QStringList &gptIniNameVariants()
{
    static const QStringList variants = { "GPT.INI", "gpt.ini", "Gpt.ini" };

    return variants;
}

bool isSmbPath(const QString &path)
{
    return path.startsWith("smb://");
}

QString directoryOf(const QString &path)
{
    const int separator = path.lastIndexOf('/');

    return separator < 0 ? QString() : path.left(separator);
}

bool canBeOpenedForReading(const QString &path)
{
    if (isSmbPath(path))
    {
        smb::SmbFile file(path);

        if (!file.open(QFile::ReadOnly))
        {
            return false;
        }

        file.close();

        return true;
    }

    return QFile::exists(path);
}

// Returns the spelling of GPT.INI that actually exists next to the requested one or an empty
// string if there is none.
QString findExistingGptIni(const QString &path)
{
    if (canBeOpenedForReading(path))
    {
        return path;
    }

    const QString directory = directoryOf(path);

    if (directory.isEmpty())
    {
        return QString();
    }

    for (const QString &name : gptIniNameVariants())
    {
        const QString candidate = directory + "/" + name;

        if (candidate != path && canBeOpenedForReading(candidate))
        {
            return candidate;
        }
    }

    return QString();
}

bool readWholeFile(const QString &path, std::string &contents, QString &errorString)
{
    contents.clear();

    if (isSmbPath(path))
    {
        smb::SmbFile file(path);

        if (!file.open(QFile::ReadOnly))
        {
            errorString = file.errorString();

            return false;
        }

        const qint64 size = file.size();

        if (size > 0)
        {
            contents.resize(static_cast<std::size_t>(size), 0);

            qint64 totalRead = 0;

            // A single read is allowed to return less than requested.
            while (totalRead < size)
            {
                const qint64 bytesRead = file.read(contents.data() + totalRead, size - totalRead);

                if (bytesRead < 0)
                {
                    errorString = file.errorString();
                    contents.clear();
                    file.close();

                    return false;
                }

                if (bytesRead == 0)
                {
                    break;
                }

                totalRead += bytesRead;
            }

            contents.resize(static_cast<std::size_t>(totalRead));
        }

        file.close();

        return true;
    }

    QFile file(path);

    if (!file.open(QFile::ReadOnly))
    {
        errorString = file.errorString();

        return false;
    }

    const QByteArray data = file.readAll();
    contents.assign(data.constData(), static_cast<std::size_t>(data.size()));
    file.close();

    return true;
}

bool writeWholeFile(const QString &path, const std::string &contents, QString &errorString)
{
    if (isSmbPath(path))
    {
        smb::SmbFile file(path);

        if (!file.open(QFile::WriteOnly | QFile::Truncate))
        {
            errorString = file.errorString();

            return false;
        }

        const qint64 size = static_cast<qint64>(contents.size());

        bool ok = true;
        qint64 totalWritten = 0;

        // A single write is allowed to consume less than requested.
        while (totalWritten < size)
        {
            const qint64 bytesWritten = file.write(contents.data() + totalWritten, size - totalWritten);

            if (bytesWritten <= 0)
            {
                errorString = file.errorString();
                ok          = false;

                break;
            }

            totalWritten += bytesWritten;
        }

        file.close();

        return ok;
    }

    QFile file(path);

    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        errorString = file.errorString();

        return false;
    }

    const qint64 size = static_cast<qint64>(contents.size());

    bool ok = true;
    qint64 totalWritten = 0;

    while (totalWritten < size)
    {
        const qint64 bytesWritten = file.write(contents.data() + totalWritten, size - totalWritten);

        if (bytesWritten <= 0)
        {
            errorString = file.errorString();
            ok          = false;

            break;
        }

        totalWritten += bytesWritten;
    }

    file.close();

    return ok;
}

void showFileError(const QString &message, const QString &path, const QString &reason)
{
    QString text = message + "\n" + path;

    if (!reason.isEmpty())
    {
        text += "\n" + reason;
    }

    QMessageBox messageBox(QMessageBox::Critical, QObject::tr("Error"), text, QMessageBox::Ok, nullptr);
    messageBox.exec();
}

}

bool GptIniUtils::onIniFileOpen(const QString &path, QString &policyName, int &machineVersion, int &userVersion)
{
    qWarning() << "Path recieved: " << path;

    const QString existingPath = findExistingGptIni(path);

    if (existingPath.isEmpty())
    {
        // A policy that has never been edited has no GPT.INI yet. This is not an error, the
        // file is created on the first save with the default versions.
        qWarning() << "No GPT.INI found for policy: " << path << " assuming a new one.";

        return false;
    }

    std::string stringvalues;
    QString errorString;

    if (!readWholeFile(existingPath, stringvalues, errorString))
    {
        qWarning() << "Unable to read" << existingPath << ":" << errorString;

        showFileError(QObject::tr("Error reading file:"), existingPath, errorString);

        return false;
    }

    try
    {
        auto iss = std::make_unique<std::istringstream>(stringvalues);
        std::string pluginName("ini_ascii");

        auto reader  = std::make_unique<io::GenericReader>();
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
        qWarning() << "Unable to parse" << existingPath << ":" << e.what();

        showFileError(QObject::tr("Error reading file:"), existingPath, QString::fromLocal8Bit(e.what()));
    }

    return false;
}

bool GptIniUtils::onIniFileSave(const QString &path, const QString &policyName, const int machineVersion, const int userVersion)
{
    std::unique_ptr<io::IniFile> fileData = std::make_unique<io::IniFile>();
    fileData->addValue("General", "DisplayName", policyName.toStdString());
    int version = (userVersion << 16) + machineVersion;
    fileData->addValue("General", "Version", std::to_string(version));

    QString pluginName = "ini_ascii";

    std::unique_ptr<io::PolicyFileFormat<io::IniFile> > format(
        gpui::PluginStorage::instance()->createPluginClass<io::PolicyFileFormat<io::IniFile> >(pluginName));

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

    // An existing GPT.INI is updated in place, otherwise the canonical spelling is created.
    const QString existingPath = findExistingGptIni(path);
    const QString targetPath   = existingPath.isEmpty() ? path : existingPath;

    QString errorString;

    if (!writeWholeFile(targetPath, oss->str(), errorString))
    {
        qWarning() << "Unable to write" << targetPath << ":" << errorString;

        showFileError(QObject::tr("Error writing file:"), targetPath, errorString);

        return false;
    }

    return true;
}

}
