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

#ifndef GPUI_SECURITY_BUNDLE_H
#define GPUI_SECURITY_BUNDLE_H

#include <memory>
#include <string>

#include "../sdmx/security.h"

class QFileInfo;
class QString;
class QStandardItem;
class QStandardItemModel;
class QAbstractItemModel;
class QModelIndex;

namespace security
{

class SecurityBundlePrivate;

class SecurityBundle
{
public:
    SecurityBundle();
    ~SecurityBundle();

    std::unique_ptr<QStandardItemModel> loadFolder(const std::string &path, const std::string &language);

private:
    SecurityBundle(const SecurityBundle &) = delete;            // copy ctor
    SecurityBundle(SecurityBundle &&)      = delete;            // move ctor
    SecurityBundle &operator=(const SecurityBundle &) = delete; // copy assignment
    SecurityBundle &operator=(SecurityBundle &&) = delete;      // move assignment

private:
    SecurityBundlePrivate *d { nullptr };

private:
    bool loadSdmxAndSdml(const QFileInfo &sdmxFileName);

    QString constructFileName(const QFileInfo &fileName);


    void rearrangeTreeItems();
    void assignParentCategory(const std::string &rawCategory,
                              QStandardItem *machineItem,
                              QStandardItem *userItem,
                              const std::string &fileName);

    QStandardItem *createItem(const QString &displayName,
                              const QString &iconName,
                              const QString &explainText,
                              const uint itemType,
                              const SecurityType securityType,
                              const bool alreadyInserted);

    void assignSupportedOn();

    void removeEmptyItems();

    void iterateModelAndRemoveEmptyFolders(QAbstractItemModel *model, const QModelIndex &parent);
};

} // namespace security

#endif // GPUI_SECURITY_BUNDLE_H
