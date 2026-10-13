/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#include "securitysnapin.h"

#include <gui/mainwindow.h>

#include "model/ui/securityproxymodel.h"
#include "model/bundle/securitybundle.h"

#include "../storage/smb/smbdirectory.h"


using namespace security;

namespace gpui
{
class SecuritySnapInPrivate
{
public:
    std::string sdmxPath   = "/usr/share/PolicyDefinitions/";
    std::string localeName = "en-US";
    std::string policyPath = "";

    std::unique_ptr<QStandardItemModel> model      = nullptr;
    std::unique_ptr<SecurityProxyModel> proxyModel = nullptr;

    void onDataSave()
    {
        createDirectory(policyPath + "/User/");
        createDirectory(policyPath + "/Machine/");
    }

    void policyBundleLoad()
    {
        auto bundle = std::make_unique<security::SecurityBundle>();
        model       = bundle->loadFolder(sdmxPath, localeName);
        proxyModel->setSourceModel(model.get());
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

    SecuritySnapInPrivate() {}

private:
    SecuritySnapInPrivate(const SecuritySnapInPrivate&)            = delete;   // copy ctor
    SecuritySnapInPrivate(SecuritySnapInPrivate&&)                 = delete;   // move ctor
    SecuritySnapInPrivate& operator=(const SecuritySnapInPrivate&) = delete;   // copy assignment
    SecuritySnapInPrivate& operator=(SecuritySnapInPrivate&&)      = delete;   // move assignment

};

SecuritySnapIn::SecuritySnapIn()
    : AbstractSnapIn("ISnapIn",
                     "SecuritySnapIn",
                     "Snap-in for security management.",
                     {1, 0, 0},
                     "GPL-2.0",
                     "Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>")
    , d(new SecuritySnapInPrivate())
{
}

void SecuritySnapIn::onInitialize(QMainWindow *window)
{
    auto mainWindow = dynamic_cast<::gpui::MainWindow *>(window);

    if (mainWindow)
    {
        d->sdmxPath   = mainWindow->getAdmxPath().toStdString();
        d->localeName = mainWindow->getLanguage().toStdString();
        qWarning() << "Setting default settings for security templates snap-in: "
                   << d->sdmxPath.c_str()
                   << d->localeName.c_str();
    }

    d->proxyModel    = std::make_unique<SecurityProxyModel>();

    d->policyBundleLoad();

    QObject::connect(d->proxyModel.get(), &SecurityProxyModel::savePolicyChanges, [&]() {
        d->onDataSave();
    });

    setRootNode(static_cast<QAbstractItemModel *>(d->proxyModel.get()));

    if (mainWindow)
    {
        QObject::connect(mainWindow, &MainWindow::admxPathChanged, [&](const QString &admxPath) {
            qWarning() << "Loading bundle from snap-in: " << admxPath;
            d->sdmxPath = admxPath.toStdString();
            d->policyBundleLoad();
        });

        QObject::connect(d->proxyModel.get(),
                         &SecurityProxyModel::savePolicyChanges,
                         mainWindow,
                         &MainWindow::updateStatusBar);
    }
}

void SecuritySnapIn::onShutdown()
{
}

void SecuritySnapIn::onDataLoad(const std::string &policyPath, const std::string &locale)
{
    Q_UNUSED(locale);

    if (!policyPath.empty())
    {
        d->policyPath = policyPath;
    }
}

void SecuritySnapIn::onDataSave()
{
    d->onDataSave();
}

void SecuritySnapIn::onRetranslateUI(const std::string &locale)
{
    d->localeName = locale;
    d->policyBundleLoad();
    setRootNode(static_cast<QAbstractItemModel *>(d->proxyModel.get()));
}

}
