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
    Q_UNUSED(admxFileName);
    Q_UNUSED(language);
    Q_UNUSED(fallbackLanguage);

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
