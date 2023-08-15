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

#include "securitybundle.h"

#include <QDebug>
#include <QStandardItemModel>

namespace security
{

SecurityBundle::SecurityBundle()
{

}

SecurityBundle::~SecurityBundle()
{

}

std::unique_ptr<QStandardItemModel> SecurityBundle::loadFolder(const std::string &path, const std::string &language)
{
    Q_UNUSED(path);
    Q_UNUSED(language);

    return nullptr;
}

bool SecurityBundle::loadAdmxAndAdml(const QFileInfo &admxFileName)
{
    Q_UNUSED(admxFileName);

    return false;
}

QString SecurityBundle::constructFileName(const QFileInfo &fileName)
{
    Q_UNUSED(fileName);

    return "";
}

void SecurityBundle::rearrangeTreeItems()
{
}

void SecurityBundle::assignParentCategory(const std::string &rawCategory,
                                          QStandardItem *machineItem,
                                          QStandardItem *userItem,
                                          const std::string &fileName)
{
    Q_UNUSED(rawCategory);
    Q_UNUSED(machineItem);
    Q_UNUSED(userItem);
    Q_UNUSED(fileName);
}

QStandardItem *SecurityBundle::createItem(const QString &displayName,
                                          const QString &iconName,
                                          const QString &explainText,
                                          const uint itemType,
                                          int securityType,
                                          const bool alreadyInserted)
{
    Q_UNUSED(displayName);
    Q_UNUSED(iconName);
    Q_UNUSED(explainText);
    Q_UNUSED(itemType);
    Q_UNUSED(securityType);
    Q_UNUSED(alreadyInserted);

    return nullptr;
}

void SecurityBundle::assignSupportedOn()
{
}

void SecurityBundle::removeEmptyItems()
{
}

void SecurityBundle::iterateModelAndRemoveEmptyFolders(QAbstractItemModel *model,
                                                       const QModelIndex &parent)
{
    Q_UNUSED(model);
    Q_UNUSED(parent);
}

}
