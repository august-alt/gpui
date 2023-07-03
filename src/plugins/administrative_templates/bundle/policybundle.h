/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd. <org@basealt.ru>
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

#ifndef GPUI_POLICYBUNDLE_H
#define GPUI_POLICYBUNDLE_H

#include "../administrativetemplates.h"

#include "../admx/policytype.h"
#include "../admx/supportedproduct.h"
#include "../admx/supporteddefinition.h"

#include <memory>
#include <string>
#include <unordered_map>

class QFileInfo;
class QString;
class QStandardItem;
class QStandardItemModel;
class QAbstractItemModel;
class QModelIndex;

namespace model
{
namespace admx
{
class Policy;
}

namespace bundle
{
class PolicyBundlePrivate;

class GPUI_ADMINISTRATIVE_TEMPLATES_EXPORT PolicyBundle
{
public:
    PolicyBundle();
    ~PolicyBundle();

    std::unique_ptr<QStandardItemModel> loadFolder(const std::string &path, const std::string &language);
    std::vector<std::shared_ptr<model::admx::SupportedProduct>> getProducts();
    std::unordered_map<std::string, std::shared_ptr<admx::SupportedDefinition>> getSupportedOnDefenitions();

private:
    PolicyBundle(const PolicyBundle &) = delete;            // copy ctor
    PolicyBundle(PolicyBundle &&)      = delete;            // move ctor
    PolicyBundle &operator=(const PolicyBundle &) = delete; // copy assignment
    PolicyBundle &operator=(PolicyBundle &&) = delete;      // move assignment

private:
    bool loadAdmxAndAdml(const QFileInfo &admxFileName);

    QString constructFileName(const QFileInfo &fileName);

    PolicyBundlePrivate *d;

    void rearrangeTreeItems();
    void assignParentCategory(const std::string &rawCategory,
                              QStandardItem *machineItem,
                              QStandardItem *userItem,
                              const std::string &fileName);

    QStandardItem *createItem(const QString &displayName,
                              const QString &iconName,
                              const QString &explainText,
                              const uint itemType,
                              const model::admx::PolicyType policyType,
                              const bool alreadyInserted);

    void assignSupportedOn();

    void removeEmptyItems();

    void iterateModelAndRemoveEmptyFolders(QAbstractItemModel *model, const QModelIndex &parent);
};
} // namespace bundle
} // namespace model

#endif // GPUI_POLICYBUNDLE_H
