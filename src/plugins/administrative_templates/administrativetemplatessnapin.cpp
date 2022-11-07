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

#include "../gui/mainwindow.h"

#include "../io/registryfileformat.h"

#include "../io/genericreader.h"
#include "../io/registryfile.h"
#include "../io/registryfileformat.h"

#include "../io/policydefinitionsfile.h"
#include "../io/policyfileformat.h"

#include "../io/inifile.h"

#include "../../src/plugins/storage/smb/smbdirectory.h"

#include "ui/administrativetemplatesproxymodel.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>

namespace gpui
{
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

class AdministrativeTemplatesSnapInPrivate
{
public:
    std::unique_ptr<QStandardItemModel> model                     = nullptr;
    std::unique_ptr<AdministrativeTemplatesProxyModel> proxyModel = nullptr;

    std::shared_ptr<model::registry::Registry> userRegistry{};
    std::unique_ptr<model::registry::AbstractRegistrySource> userRegistrySource{};
    QString userRegistryPath{};

    std::shared_ptr<model::registry::Registry> machineRegistry{};
    std::unique_ptr<model::registry::AbstractRegistrySource> machineRegistrySource{};
    QString machineRegistryPath{};

    std::vector<std::unique_ptr<QTranslator>> translators{};

    std::string admxPath   = "/usr/share/PolicyDefinitions/";
    std::string localeName = "en-US";
    std::string policyPath = "";

    QAction *fileAction{};

    AdministrativeTemplatesSnapInPrivate()
        : userRegistry(new model::registry::Registry())
        , userRegistrySource(new model::registry::PolRegistrySource(userRegistry))
        , machineRegistry(new model::registry::Registry())
        , machineRegistrySource(new model::registry::PolRegistrySource(machineRegistry))
    {}

public:
    void onDataSave()
    {
        createDirectory(policyPath + "/User/");
        createDirectory(policyPath + "/Machine/");

        if (!machineRegistryPath.isEmpty())
        {
            qWarning() << "Saving machine registry to: " << machineRegistryPath;
            onPolFileSave(machineRegistryPath.toStdString(), machineRegistry);
        }
        else
        {
            qWarning() << "Unable to save machine registry path is empty!";
        }

        if (!userRegistryPath.isEmpty())
        {
            qWarning() << "Saving user registry to: " << userRegistryPath;
            onPolFileSave(userRegistryPath.toStdString(), userRegistry);
        }
        else
        {
            qWarning() << "Unable to save user registry path is empty!";
        }
    }

    void createDirectory(const std::string &directoryName)
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

    void policyBundleLoad()
    {
        auto bundle = std::make_unique<model::bundle::PolicyBundle>();
        model       = bundle->loadFolder(admxPath, localeName);
        proxyModel->setSourceModel(model.get());
    }

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

AdministrativeTemplatesSnapIn::AdministrativeTemplatesSnapIn()
    : AbstractSnapIn("ISnapIn",
                     "AdministrativeTemplatesSnapIn",
                     "Snap-in for preferences management.",
                     {1, 0, 0},
                     "GPL-2.0",
                     "Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>")
    , d(new AdministrativeTemplatesSnapInPrivate())
{}

void AdministrativeTemplatesSnapIn::onInitialize(QMainWindow *window)
{
    auto mainWindow = dynamic_cast<::gpui::MainWindow *>(window);

    if (mainWindow)
    {
        d->admxPath   = mainWindow->getAdmxPath().toStdString();
        d->localeName = mainWindow->getLanguage().toStdString();
        qWarning() << "Setting default settings for administrative templates snap-in: " << d->admxPath.c_str()
                   << d->localeName.c_str();
    }

    d->proxyModel = std::make_unique<AdministrativeTemplatesProxyModel>();

    d->policyBundleLoad();

    QObject::connect(d->proxyModel.get(), &AdministrativeTemplatesProxyModel::savePolicyChanges, [&]() {
        d->onDataSave();
    });

    setRootNode(static_cast<QAbstractItemModel *>(d->proxyModel.get()));

    if (mainWindow)
    {
        QObject::connect(mainWindow, &MainWindow::admxPathChanged, [&](const QString &admxPath) {
            d->admxPath = admxPath.toStdString();
            d->policyBundleLoad();
        });

        QObject::connect(d->proxyModel.get(),
                         &AdministrativeTemplatesProxyModel::savePolicyChanges,
                         mainWindow,
                         &MainWindow::updateStatusBar);
    }

    std::cout << std::string(__PRETTY_FUNCTION__) << std::endl;
}

void AdministrativeTemplatesSnapIn::onShutdown()
{
    std::cout << std::string(__PRETTY_FUNCTION__) << std::endl;
}

void AdministrativeTemplatesSnapIn::onDataLoad(const std::string &policyPath, const std::string &locale)
{
    Q_UNUSED(locale);

    if (!policyPath.empty())
    {
        d->policyPath = policyPath;

        d->userRegistryPath    = QString::fromStdString(policyPath) + "/User/Registry.pol";
        d->machineRegistryPath = QString::fromStdString(policyPath) + "/Machine/Registry.pol";

        onPolFileOpen(d->userRegistryPath,
                      d->userRegistry,
                      d->userRegistrySource,
                      [&](model::registry::AbstractRegistrySource *source) noexcept {
                          d->proxyModel->setUserRegistrySource(source);
                      });

        onPolFileOpen(d->machineRegistryPath,
                      d->machineRegistry,
                      d->machineRegistrySource,
                      [&](model::registry::AbstractRegistrySource *source) noexcept {
                          d->proxyModel->setMachineRegistrySource(source);
                      });
    }
}

void AdministrativeTemplatesSnapIn::onDataSave()
{
    d->onDataSave();
}

void AdministrativeTemplatesSnapIn::onRetranslateUI(const std::string &locale)
{
    for (const auto &translator : d->translators)
    {
        QCoreApplication::removeTranslator(translator.get());
    }
    d->translators.clear();

    QString language = QString::fromStdString(locale).split("-")[0];

    QDirIterator it(":/", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        if (!it.fileInfo().isFile())
        {
            it.hasNext();
        }

        if (it.fileName().endsWith(language + ".qm"))
        {
            std::unique_ptr<QTranslator> qtTranslator = std::make_unique<QTranslator>();
            bool loadResult                           = qtTranslator->load(it.fileName(), ":/");
            if (loadResult)
            {
                QCoreApplication::installTranslator(qtTranslator.get());
                d->translators.push_back(std::move(qtTranslator));
            }
        }

        it.next();
    }

    d->localeName = locale;
    d->policyBundleLoad();
    setRootNode(static_cast<QAbstractItemModel *>(d->proxyModel.get()));
}

} // namespace gpui
