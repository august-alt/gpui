/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd.
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

#include "policybundle.h"

#include "../pluginstorage.h"

#include "../../plugins/admx/admxformat.h"
#include "../../plugins/adml/admlformat.h"
#include "../../io/genericfile.h"

#include "../admx/policydefinitions.h"
#include "../presentation/policyresources.h"

#include <QDir>
#include <QStandardItemModel>

#include <fstream>

namespace model {

namespace bundle {

class PolicyBundlePrivate
{
public:
    std::unique_ptr<QStandardItemModel> treeModel;
};

PolicyBundle::PolicyBundle()
    : d(new PolicyBundlePrivate())
{
}

std::unique_ptr<QStandardItemModel> PolicyBundle::loadFolder(const std::string& path, const std::string& language,
                                                          const std::string& fallbackLanguage)
{
    d->treeModel = std::make_unique<QStandardItemModel>();

    const QDir dir(path.c_str());
    const QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    for (const QFileInfo& file : files) {
        if (file.fileName().toLower().endsWith(".admx")) {
            loadAdmxAndAdml(file, language, fallbackLanguage);
        }
    }

    return std::move(d->treeModel);
}

template<typename TPolicies, typename TFormat>
std::unique_ptr<TPolicies> loadPolicies(const QString& pluginName, const QFileInfo& admxFileName)
{
    std::unique_ptr<TPolicies> policies;

    TFormat* format = gpui::PluginStorage::instance()->createPluginClass<TFormat>(pluginName);

    if (!format)
    {
        return policies;
    }

    std::ifstream file;

    file.open(admxFileName.absoluteFilePath().toStdString(), std::ifstream::in);

    if (file.good()) {
        policies = std::make_unique<TPolicies>();

        if (!format->read(file, policies.get()))
        {
            qWarning() << admxFileName.fileName() + " " + QString::fromStdString(format->getErrorString());
        }
    }

    file.close();

    return policies;
}

QString PolicyBundle::constructFileName(const QFileInfo& fileName, const std::string& language)
{
    QString admlFileName = fileName.fileName();
    admlFileName.replace(admlFileName.length() - 4, 4, "adml");
    admlFileName.prepend(QDir::separator() + QString::fromStdString(language) + QDir::separator());
    admlFileName.prepend(fileName.absolutePath());
    return admlFileName;
}

bool PolicyBundle::loadAdmxAndAdml(const QFileInfo& admxFileName, const std::string& language,
                                   const std::string& fallbackLanguage)
{
    Q_UNUSED(fallbackLanguage);

    auto policyDefinitions = loadPolicies<io::PolicyDefinitionsFile, io::PolicyFileFormat<io::PolicyDefinitionsFile>>("admx", admxFileName);
    if (!policyDefinitions.get())
    {
        return false;
    }
    QString admlFileName = constructFileName(admxFileName, language);
    auto policyResources = loadPolicies<io::PolicyResourcesFile, io::PolicyFileFormat<io::PolicyResourcesFile>>("adml", admlFileName);
    if (!policyResources.get())
    {
        return false;
    }

    QStandardItem * rootItem = d->treeModel->invisibleRootItem();

    for (auto& definition : policyDefinitions->getAllPolicyDefinitions())
    {
        for (auto& categories : definition->categories)
        {
            QString displayName = QString::fromStdString(categories->displayName).replace("$(string.", "");
            displayName.replace(")", "");
            for (auto& policyResource : policyResources->getAll())
            {
                if (policyResource->stringTable.find(displayName.toStdString()) != policyResource->stringTable.end())
                {
                    displayName = QString::fromStdString(policyResource->stringTable[displayName.toStdString()]);
                }
            }

            rootItem->appendRow(new QStandardItem(displayName));
        }
    }

    // TODO: Load admx and adml files.
    return true;
}

}

}
