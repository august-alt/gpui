/***********************************************************************************************************************
**
** Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>
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

#include "administrativetemplatessnapin.h"

#include <iostream>

#include <QAbstractItemModel>
#include <QStandardItemModel>

#include "bundle/policybundle.h"

#include "registry/abstractregistrysource.h"
#include "registry/polregistrysource.h"
#include "registry/registry.h"

#include "../plugins/storage/smb/smbfile.h"

#include "../io/registryfileformat.h"

#include "../io/genericreader.h"
#include "../io/registryfile.h"
#include "../io/registryfileformat.h"

#include "../io/policydefinitionsfile.h"
#include "../io/policyfileformat.h"

#include "../io/inifile.h"

#include <QDebug>
#include <QMessageBox>

namespace gpui
{
class AdministrativeTemplatesSnapInPrivate
{
public:
    std::unique_ptr<QStandardItemModel> model = nullptr;

    std::shared_ptr<model::registry::Registry> userRegistry{};
    std::unique_ptr<model::registry::AbstractRegistrySource> userRegistrySource{};
    QString userRegistryPath{};

    std::shared_ptr<model::registry::Registry> machineRegistry{};
    std::unique_ptr<model::registry::AbstractRegistrySource> machineRegistrySource{};
    QString machineRegistryPath{};

    AdministrativeTemplatesSnapInPrivate()
        : userRegistry(new model::registry::Registry())
        , userRegistrySource(new model::registry::PolRegistrySource(userRegistry))
        , machineRegistry(new model::registry::Registry())
        , machineRegistrySource(new model::registry::PolRegistrySource(machineRegistry))
    {}

private:
    AdministrativeTemplatesSnapInPrivate(const AdministrativeTemplatesSnapInPrivate &) = delete; // copy ctor
    AdministrativeTemplatesSnapInPrivate(AdministrativeTemplatesSnapInPrivate &&)      = delete; // move ctor
    AdministrativeTemplatesSnapInPrivate &operator=(const AdministrativeTemplatesSnapInPrivate &)
        = delete; // copy assignment
    AdministrativeTemplatesSnapInPrivate &operator=(AdministrativeTemplatesSnapInPrivate &&) = delete; // move assignment
};

void onPolFileOpen(const QString &path,
                   std::shared_ptr<model::registry::Registry> &registry,
                   std::unique_ptr<model::registry::AbstractRegistrySource> &source,
                   std::function<void(model::registry::AbstractRegistrySource *)> callback)
{
    qWarning() << "Path recieved: " << path;

    auto stringvalues = std::make_unique<std::string>();

    try
    {
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
        std::string pluginName("pol");

        auto reader       = std::make_unique<io::GenericReader>();
        auto registryFile = reader->load<io::RegistryFile, io::RegistryFileFormat<io::RegistryFile>>(*iss, pluginName);
        if (!registryFile)
        {
            qWarning() << "Unable to load registry file contents.";
            return;
        }

        registry = registryFile->getRegistry();

        source = std::make_unique<model::registry::PolRegistrySource>(registry);

        callback(source.get());
    }
    catch (std::exception &e)
    {
        qWarning() << "Warning: Unable to read file: " << qPrintable(path) << " description: " << e.what();
    }
}

void onPolFileSave(const std::string &fileName, std::shared_ptr<model::registry::Registry> registry)
{
    std::unique_ptr<io::RegistryFile> fileData = std::make_unique<io::RegistryFile>();
    fileData->setRegistry(registry);

    QString pluginName = QString::fromStdString(fileName);
    pluginName         = pluginName.mid(pluginName.lastIndexOf('.') + 1);

    io::RegistryFileFormat<io::RegistryFile> *format
        = gpui::PluginStorage::instance()->createPluginClass<io::RegistryFileFormat<io::RegistryFile>>(pluginName);

    if (!format)
    {
        qWarning() << "Format supporting: " << pluginName << " not found.";

        return;
    }

    auto oss = std::make_unique<std::ostringstream>();

    if (!format->write(*oss, fileData.get()))
    {
        qWarning() << fileName.c_str() << " " << format->getErrorString().c_str();
    }

    oss->flush();

    qWarning() << "Current string values." << oss->str().c_str();

    bool ifShowError = false;

    auto showMessageFunction = [&fileName]() {
        QMessageBox messageBox(QMessageBox::Critical,
                               QObject::tr("Error"),
                               QObject::tr("Error writing file:") + "\n" + qPrintable(fileName.c_str()),
                               QMessageBox::Ok);
        messageBox.exec();
    };

    try
    {
        if (QString::fromStdString(fileName).startsWith("smb://"))
        {
            gpui::smb::SmbFile smbLocationItemFile(QString::fromStdString(fileName));
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
            QFile registryFile(QString::fromStdString(fileName));
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
    }

    if (!ifShowError)
    {
        showMessageFunction();
    }

    delete format;
}

AdministrativeTemplatesSnapIn::AdministrativeTemplatesSnapIn()
    : AbstractSnapIn("ISnapIn",
                     "AdministrativeTemplatesSnapIn",
                     "Snap-in for preferences management.",
                     {1, 0, 0},
                     "GPL-2.0",
                     "Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>")
    , d(new AdministrativeTemplatesSnapInPrivate())
{}

void AdministrativeTemplatesSnapIn::onInitialize()
{
    auto bundle = std::make_unique<model::bundle::PolicyBundle>();
    d->model    = bundle->loadFolder("/usr/share/PolicyDefinitions/", "ru-ru");

    setRootNode(static_cast<QAbstractItemModel *>(d->model.get()));

    // TODO: Register action to load model from path.

    std::cout << std::string(__PRETTY_FUNCTION__) << std::endl;
}

void AdministrativeTemplatesSnapIn::onShutdown()
{
    std::cout << std::string(__PRETTY_FUNCTION__) << std::endl;
}

void AdministrativeTemplatesSnapIn::onDataLoad(const std::string &policyPath, const std::string &locale)
{
    Q_UNUSED(policyPath);
    Q_UNUSED(locale);

    if (!policyPath.empty())
    {
        d->userRegistryPath    = QString::fromStdString(policyPath) + "/User/Registry.pol";
        d->machineRegistryPath = QString::fromStdString(policyPath) + "/Machine/Registry.pol";

        onPolFileOpen(d->userRegistryPath,
                      d->userRegistry,
                      d->userRegistrySource,
                      [&](model::registry::AbstractRegistrySource *source) noexcept {
                          Q_UNUSED(source);
                          // TODO: set registry source to content widget.
                      });

        onPolFileOpen(d->machineRegistryPath,
                      d->machineRegistry,
                      d->machineRegistrySource,
                      [&](model::registry::AbstractRegistrySource *source) noexcept {
                          Q_UNUSED(source);
                          // TODO: set registry source to content widget.
                      });
    }
}

void AdministrativeTemplatesSnapIn::onDataSave()
{
    if (!d->machineRegistryPath.isEmpty())
    {
        qWarning() << "Saving machine registry to: " << d->machineRegistryPath;
        onPolFileSave(d->machineRegistryPath.toStdString(), d->machineRegistry);
    }
    else
    {
        qWarning() << "Unable to save machine registry path is empty!";
    }

    if (!d->userRegistryPath.isEmpty())
    {
        qWarning() << "Saving user registry to: " << d->userRegistryPath;
        onPolFileSave(d->userRegistryPath.toStdString(), d->userRegistry);
    }
    else
    {
        qWarning() << "Unable to save user registry path is empty!";
    }
}

void AdministrativeTemplatesSnapIn::onRetranslateUI(const std::string &locale)
{
    Q_UNUSED(locale);
}

} // namespace gpui
