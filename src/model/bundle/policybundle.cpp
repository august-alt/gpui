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

#include <QDir>

#include <fstream>

namespace model {

namespace bundle {

PolicyBundle::PolicyBundle(const ILogger& logger)
{
    Q_UNUSED(logger);
}

std::shared_ptr<PolicyTreeModel> PolicyBundle::loadFolder(const std::string& path, const std::string& language,
                                                          const std::string& fallbackLanguage)
{
    const QDir dir(path.c_str());
    const QFileInfoList files = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    for (const QFileInfo& file : files) {
        if (file.completeBaseName().toLower().endsWith(".admx")) {
            loadAdmxAndAdml(file, language, fallbackLanguage);
        }
    }

    return std::shared_ptr<PolicyTreeModel>(nullptr);
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

        reinterpret_cast<io::PolicyFileFormat<TPolicies>*>(format)->read(file, policies.get());
    }

    file.close();

    return policies;
}

bool PolicyBundle::loadAdmxAndAdml(const QFileInfo& admxFileName, const std::string& language,
                                   const std::string& fallbackLanguage)
{
    Q_UNUSED(admxFileName);
    Q_UNUSED(language);
    Q_UNUSED(fallbackLanguage);

    auto policyDefinitions = loadPolicies<io::PolicyDefinitionsFile, gpui::AdmxFormat>("admx", admxFileName);

    QString admlFilePath = admxFileName.absolutePath();
    admlFilePath.replace(admlFilePath.length() - 4, 4, "adml");

    qWarning() << admlFilePath;

    //auto policyResources = loadPolicies<io::PolicyResourcesFile, gpui::AdmlFormat>()

    // TODO: Load admx and adml files.
    return false;
}

void PolicyBundle::logError(const std::string& error)
{
    Q_UNUSED(error);
    // TODO: Implement logging.
}

}

}
