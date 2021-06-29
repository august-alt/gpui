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

#include <QDir>

#include "../../plugins/adml/admlformat.h"
#include "../../plugins/admx/admxformat.h"

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
    const QFileInfoList files = dir.entryInfoList();

    for (const QFileInfo& file : files) {
        if (file.completeBaseName().toLower().endsWith(".admx")) {
            loadAdmxAndAdml(file, language, fallbackLanguage);
        }
    }

    return std::shared_ptr<PolicyTreeModel>(nullptr);
}

bool PolicyBundle::loadAdmxAndAdml(const QFileInfo& admxFileName, const std::string& language,
                                   const std::string& fallbackLanguage)
{
    gpui::AdmxFormat admxFormat;

    std::ifstream file;

    file.open(admxFileName.path().toStdString(), std::ifstream::in);

    if (file.good())
    {
        std::unique_ptr<io::PolicyDefinitionsFile> policies = std::make_unique<io::PolicyDefinitionsFile>();

        admxFormat.read(file, policies.get());
    }

    file.close();

    QString admlFileInLanguage(admxFileName.path());
    admlFileInLanguage.prepend(QString::fromStdString(language) + QDir::separator());

    QString admlFileInFallbackLanguage(admxFileName.path());
    admlFileInFallbackLanguage.prepend(QString::fromStdString(fallbackLanguage) + QDir::separator());

    file.open(admlFileInLanguage.toStdString(), std::fstream::in);

    gpui::AdmlFormat admlFormat;

    if (file.good())
    {
        std::unique_ptr<io::PolicyResourcesFile> policies = std::make_unique<io::PolicyResourcesFile>();

        admlFormat.read(file, policies.get());
    }

    file.close();

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
