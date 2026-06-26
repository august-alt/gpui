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
#include <QDir>
#include <QStandardItemModel>
#include <QUuid>

#include "../administrative_templates/bundle/itemtype.h"
#include "../administrative_templates/bundle/policyroles.h"

#include "../sdmx/security.h"

#include "../../../src/core/pluginstorage.h"

#include "../../../src/plugins/sdml/sdmlformat.h"
#include "../../../src/plugins/sdmx/sdmxformat.h"

#include <algorithm>
#include <memory>
#include <fstream>

#include "../sdmx/securitydefinitions.h"
#include "../presentation/securitypresentationresources.h"
#include "../presentation/securitypresentation.h"

using namespace model::bundle;

namespace security
{

struct CategoryStorage
{
    QStandardItem *machineItem     = nullptr;
    QStandardItem *userItem        = nullptr;
    Category       category        = {};
    std::string fileName           = {};
};

struct SecurityStorage
{
    SecurityType type = security::SecurityType::Machine;
    std::string category         = {};
    std::string fileName         = {};
    QStandardItem *item          = nullptr;
};

class SecurityBundlePrivate
{
public:
    std::unique_ptr<QStandardItemModel> treeModel                        = nullptr;
    QStandardItem *rootMachineItem                                       = nullptr;
    QStandardItem *rootUserItem                                          = nullptr;
    std::vector<QStandardItem *> items                                   = {};
    std::map<std::string, std::string> supportedOnMap                    = {};
    QStringList languageDirectoryPaths                                   = {};
    std::vector<SecurityStorage> unassignedItems                         = {};
    std::map<std::string, CategoryStorage> categoryItemMap               = {};
};

template<typename TPolicies, typename TFormat>
std::unique_ptr<TPolicies> loadPolicies(const QString &pluginName, const QFileInfo &fileName)
{
    std::unique_ptr<TPolicies> policies;

    TFormat *format = gpui::PluginStorage::instance()->createPluginClass<TFormat>(pluginName);

    if (!format)
    {
        return policies;
    }

    std::ifstream file;

    file.open(fileName.absoluteFilePath().toStdString(), std::ifstream::in);

    if (file.good())
    {
        policies = std::make_unique<TPolicies>();

        if (!format->read(file, policies.get()))
        {
            qWarning() << fileName.fileName() + " " + QString::fromStdString(format->getErrorString());
        }
    }

    file.close();

    delete format;

    return policies;
}

QString SecurityBundle::constructFileName(const QFileInfo &fileName)
{
    QString sdmlFileName = fileName.fileName();
    sdmlFileName.replace(sdmlFileName.length() - 4, 4, "sdml");
    for (const auto &path : d->languageDirectoryPaths)
    {
        QDir admlDir(path);
        if (admlDir.isEmpty())
        {
            continue;
        }

        for (const auto &file : admlDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
        {
            if (file.fileName().toLower().compare(sdmlFileName.toLower()) == 0)
            {
                return file.absoluteFilePath();
            }
        }
    }

    return sdmlFileName;
}

std::string findStringById(const std::string &id, const std::unique_ptr<gpui::SdmlFile> &resource)
{
    if (id.length() < 10 || id.compare(0, 9, "$(string.") != 0)
    {
        return id;
    }
    std::string pureId = id.substr(9, id.length() - 10);
    for (auto &currentResource : resource->getAll())
    {
        auto search = currentResource->stringTable.find(pureId);
        if (search != currentResource->stringTable.end())
        {
            return search->second;
        }
    }

    return pureId;
}

std::shared_ptr<security::SecurityPresentation> findPresentationById(
    const std::string &id, const std::unique_ptr<gpui::SdmlFile> &resource)
{
    if (id.length() < 16 || id.compare(0, 15, "$(presentation.") != 0)
    {
        return nullptr;
    }
    std::string pureId = id.substr(15, id.length() - 16);
    for (auto &currentResource : resource->getAll())
    {
        auto &table = currentResource->presentationTable;

        auto search = std::find_if(table.begin(), table.end(), [pureId](auto& element)
        {
            return pureId.compare(element.first) == 0;
        });

        if (search != table.end())
        {
            return search->second;
        }
    }

    return nullptr;
}

void handlePresentation(const std::shared_ptr<security::SecurityPresentation> &presentation,
                        const std::shared_ptr<security::SecurityDefinition> &policy,
                        const std::unique_ptr<::gpui::SdmlFile> &policyResources)
{
    Q_UNUSED(presentation);
    Q_UNUSED(policy);
    Q_UNUSED(policyResources);
    // TODO: Implement.
}

SecurityBundle::SecurityBundle()
    : d(new SecurityBundlePrivate())
{
}

SecurityBundle::~SecurityBundle()
{
    delete d;
}

std::unique_ptr<QStandardItemModel> SecurityBundle::loadFolder(const std::string &path, const std::string &language)
{
    d->treeModel = std::make_unique<QStandardItemModel>();

    QStandardItem *rootItem        = d->treeModel->invisibleRootItem();
    QStandardItem *visibleRootItem = createItem(QObject::tr("[Local Group Policy]"),
                                                "text-x-generic-template",
                                                QObject::tr("Local group policies"),
                                                ItemType::ITEM_TYPE_CATEGORY,
                                                SecurityType::Both,
                                                false);
    visibleRootItem->setData(QUuid("{123e4567-e89b-12d3-a456-426652340003}"), model::bundle::CURRENT_UUID);

    rootItem->appendRow(visibleRootItem);

    QStandardItem *machineItem = createItem(QObject::tr("Machine"),
                                            "computer",
                                            QObject::tr("Machine level policies"),
                                            ItemType::ITEM_TYPE_CATEGORY,
                                            SecurityType::Machine,
                                            false);
    machineItem->setData(QUuid("{123e4567-e89b-12d3-a456-426652340003}"), model::bundle::PARENT_UUID);
    machineItem->setData(QUuid("{123e4567-e89b-12d3-a456-426652340000}"), model::bundle::CURRENT_UUID);
    QStandardItem *machineSystemSettingsItem = createItem(QObject::tr("System settings"),
                                                          "folder",
                                                          QObject::tr("System settings for computer"),
                                                          ItemType::ITEM_TYPE_CATEGORY,
                                                          SecurityType::Machine,
                                                          false);
    machineSystemSettingsItem->setData(QUuid("{123e4567-e89b-12d3-a456-426652340000}"), model::bundle::PARENT_UUID);
    machineSystemSettingsItem->setData(QUuid("{123e4567-e89b-12d3-a456-426652340010}"), model::bundle::CURRENT_UUID);
    machineItem->appendRow(machineSystemSettingsItem);
    d->rootMachineItem = createItem(QObject::tr("Security Settings"),
                                         "folder",
                                         QObject::tr("Machine level security settings"),
                                         ItemType::ITEM_TYPE_CATEGORY,
                                         SecurityType::User,
                                         false);
    machineSystemSettingsItem->appendRow(d->rootMachineItem);

    QStandardItem *userItem = createItem(QObject::tr("User"),
                                         "user-home",
                                         QObject::tr("User level policies"),
                                         ItemType::ITEM_TYPE_CATEGORY,
                                         SecurityType::User,
                                         false);
    userItem->setData(QUuid("{123e4567-e89b-12d3-a456-426652340003}"), model::bundle::PARENT_UUID);
    userItem->setData(QUuid("{123e4567-e89b-12d3-a456-426652340001}"), model::bundle::CURRENT_UUID);
    QStandardItem *userSystemSettingsItem = createItem(QObject::tr("System settings"),
                                              "folder",
                                              QObject::tr("System settings for user"),
                                              ItemType::ITEM_TYPE_CATEGORY,
                                              SecurityType::User,
                                              false);
    userSystemSettingsItem->setData(QUuid("{123e4567-e89b-12d3-a456-426652340001}"), model::bundle::PARENT_UUID);
    userSystemSettingsItem->setData(QUuid("{123e4567-e89b-12d3-a456-426652340011}"), model::bundle::CURRENT_UUID);
    userItem->appendRow(userSystemSettingsItem);
    d->rootUserItem = createItem(QObject::tr("Security Settings"),
                                         "folder",
                                         QObject::tr("User level security settings"),
                                         ItemType::ITEM_TYPE_CATEGORY,
                                         SecurityType::User,
                                         false);
    userSystemSettingsItem->appendRow(d->rootUserItem);

    visibleRootItem->appendRow(machineItem);
    visibleRootItem->appendRow(userItem);

    const QDir dir(path.c_str());
    const QFileInfoList files       = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    const QFileInfoList directories = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    const QString qLanguage = QString::fromStdString(language).toLower();

    d->languageDirectoryPaths.clear();

    for (const QFileInfo &subDir : directories)
    {
        if (subDir.fileName().toLower().endsWith(qLanguage))
        {
            d->languageDirectoryPaths.append(subDir.absoluteFilePath());
        }
    }

    for (const QFileInfo &file : files)
    {
        if (file.fileName().toLower().endsWith(".sdmx"))
        {
            loadSdmxAndSdml(file);
        }
    }

    rearrangeTreeItems();
    assignSupportedOn();

    removeEmptyItems();

    return std::move(d->treeModel);
}

bool SecurityBundle::loadSdmxAndSdml(const QFileInfo &sdmxFileName)
{
    auto securityDefinitions
        = loadPolicies<gpui::SdmxFile, io::PolicyFileFormat<::gpui::SdmxFile>>("sdmx", sdmxFileName);
    if (!securityDefinitions.get())
    {
        return false;
    }

    QString admlFileName = constructFileName(sdmxFileName);
    auto policyResources
        = loadPolicies<::gpui::SdmlFile, io::PolicyFileFormat<::gpui::SdmlFile>>("sdml", admlFileName);
    if (!policyResources.get())
    {
        return false;
    }

    std::string fileName = sdmxFileName.fileName().toStdString();

    for (auto &definition : securityDefinitions->getAll())
    {
        for (auto &category : definition->categories)
        {
            auto categoryName = category->name + "." + fileName;

            QString displayName = QString::fromStdString(findStringById(category->displayName, policyResources));
            QString explainText = QString::fromStdString(findStringById(category->explainText, policyResources));

            d->categoryItemMap[categoryName].machineItem = createItem(displayName,
                                                                      "folder",
                                                                      explainText,
                                                                      ItemType::ITEM_TYPE_CATEGORY,
                                                                      SecurityType::Machine,
                                                                      false);
            d->categoryItemMap[categoryName].userItem    = createItem(displayName,
                                                                   "folder",
                                                                   explainText,
                                                                   ItemType::ITEM_TYPE_CATEGORY,
                                                                   SecurityType::User,
                                                                   false);
            d->categoryItemMap[categoryName].category    = *category;
            d->categoryItemMap[categoryName].fileName    = fileName;

            d->categoryItemMap[category->name].machineItem = d->categoryItemMap[categoryName].machineItem;
            d->categoryItemMap[category->name].userItem    = d->categoryItemMap[categoryName].userItem;

            if (category->parentCategory.size() == 0)
            {
                d->rootUserItem->appendRow(d->categoryItemMap[categoryName].userItem);
                d->rootMachineItem->appendRow(d->categoryItemMap[categoryName].machineItem);
            }
        }

        for (auto &policy : definition->security)
        {

            QString displayName = QString::fromStdString(findStringById(policy->displayName, policyResources));
            QString explainText = QString::fromStdString(findStringById(policy->explainText, policyResources));

            auto policyItem = createItem(displayName,
                                         "text-x-generic",
                                         explainText,
                                         ItemType::ITEM_TYPE_POLICY,
                                         policy->securityType,
                                         false);

            SecurityStorage container;
            container.category = policy->parentCategory;
            container.item     = policyItem;
            container.type     = policy->securityType;
            container.fileName = sdmxFileName.fileName().toStdString();

            if (policy->presentation)
            {
                auto presentation = findPresentationById(*policy->presentation.get(), policyResources);
                if (presentation)
                {
                    policyItem->setData(QVariant::fromValue(presentation), PolicyRoles::PRESENTATION);
                    handlePresentation(presentation, policy, policyResources);
                }
            }

            policyItem->setData(QVariant::fromValue(policy), PolicyRoles::POLICY);

            d->unassignedItems.push_back(container);
        }
    }

    return true;
}

void SecurityBundle::rearrangeTreeItems()
{
    for (const auto &entry : d->categoryItemMap)
    {
        assignParentCategory(entry.second.category.parentCategory,
                             entry.second.machineItem,
                             entry.second.userItem,
                             entry.second.fileName);
    }

    for (const auto &item : d->unassignedItems)
    {
        if (item.type == SecurityType::User)
        {
            assignParentCategory(item.category, nullptr, item.item, item.fileName);
        }
        else if (item.type == SecurityType::Machine)
        {
            assignParentCategory(item.category, item.item, nullptr, item.fileName);
        }
        else
        {
            item.item->setData(static_cast<uint32_t>(SecurityType::Machine), PolicyRoles::POLICY_TYPE);
            QStandardItem *copyItem = createItem(item.item->text(),
                                                 "text-x-generic",
                                                 item.item->data(PolicyRoles::EXPLAIN_TEXT).value<QString>(),
                                                 ItemType::ITEM_TYPE_POLICY,
                                                 SecurityType::User,
                                                 true);
            copyItem->setData(item.item->data(PolicyRoles::SUPPORTED_ON), PolicyRoles::SUPPORTED_ON);
            copyItem->setData(item.item->data(PolicyRoles::PRESENTATION), PolicyRoles::PRESENTATION);
            copyItem->setData(item.item->data(PolicyRoles::POLICY), PolicyRoles::POLICY);
            assignParentCategory(item.category, item.item, copyItem, item.fileName);
        }
    }
}

void SecurityBundle::assignParentCategory(const std::string &rawCategory,
                                          QStandardItem *machineItem,
                                          QStandardItem *userItem,
                                          const std::string &fileName)
{
    std::string parentCategory      = rawCategory;
    std::string::size_type position = parentCategory.find(':');
    if (position != std::string::npos)
    {
        parentCategory = parentCategory.substr(position + 1);
    }

    std::string parentCategoryWithFilename = parentCategory + "." + fileName;

    auto search = d->categoryItemMap.find(parentCategoryWithFilename);
    if (search != d->categoryItemMap.end())
    {
        if (machineItem)
        {
            if (!machineItem->data(PolicyRoles::POLICY_WIDGET + 1).value<bool>())
            {
                search->second.machineItem->appendRow(machineItem);
                machineItem->setData(true, PolicyRoles::POLICY_WIDGET + 1);
            }
        }
        if (userItem)
        {
            if (!userItem->data(PolicyRoles::POLICY_WIDGET + 1).value<bool>())
            {
                search->second.userItem->appendRow(userItem);
                userItem->setData(true, PolicyRoles::POLICY_WIDGET + 1);
            }
        }
    }
    else if ((search = d->categoryItemMap.find(parentCategory)) != d->categoryItemMap.end())
    {
        if (machineItem)
        {
            if (!machineItem->data(PolicyRoles::POLICY_WIDGET + 1).value<bool>())
            {
                search->second.machineItem->appendRow(machineItem);
                machineItem->setData(true, PolicyRoles::POLICY_WIDGET + 1);
            }
        }
        if (userItem)
        {
            if (!userItem->data(PolicyRoles::POLICY_WIDGET + 1).value<bool>())
            {
                search->second.userItem->appendRow(userItem);
                userItem->setData(true, PolicyRoles::POLICY_WIDGET + 1);
            }
        }
    }
    else if (rawCategory.size() > 0)
    {
        qWarning() << "Unable to find parent category: " << rawCategory.c_str() << fileName.c_str();
        if (machineItem)
        {
            if (!machineItem->data(PolicyRoles::POLICY_WIDGET + 1).value<bool>())
            {
                d->rootMachineItem->appendRow(machineItem);
                machineItem->setData(true, PolicyRoles::POLICY_WIDGET + 1);
            }
        }

        if (userItem)
        {
            if (!userItem->data(PolicyRoles::POLICY_WIDGET + 1).value<bool>())
            {
                d->rootUserItem->appendRow(userItem);
                userItem->setData(true, PolicyRoles::POLICY_WIDGET + 1);
            }
        }
    }
}

QStandardItem *SecurityBundle::createItem(const QString &displayName,
                                          const QString &iconName,
                                          const QString &explainText,
                                          const uint itemType,
                                          const SecurityType securityType,
                                          const bool alreadyInserted)
{
    QStandardItem *categoryItem = new QStandardItem(displayName.trimmed());
    categoryItem->setIcon(QIcon::fromTheme(iconName));
    categoryItem->setFlags(categoryItem->flags() & (~Qt::ItemIsEditable));
    categoryItem->setData(explainText, PolicyRoles::EXPLAIN_TEXT);
    categoryItem->setData(itemType, PolicyRoles::ITEM_TYPE);
    categoryItem->setData(static_cast<uint32_t>(securityType), PolicyRoles::POLICY_TYPE);
    categoryItem->setData(alreadyInserted, PolicyRoles::POLICY_WIDGET + 1);

    d->items.push_back(categoryItem);

    return categoryItem;
}

void SecurityBundle::assignSupportedOn()
{
}

void SecurityBundle::removeEmptyItems()
{
    iterateModelAndRemoveEmptyFolders(d->treeModel.get(), d->rootMachineItem->index());
    iterateModelAndRemoveEmptyFolders(d->treeModel.get(), d->rootUserItem->index());
}

void SecurityBundle::iterateModelAndRemoveEmptyFolders(QAbstractItemModel *model,
                                                       const QModelIndex &parent)
{
    for (int r = 0; r < model->rowCount(parent); ++r)
    {
        QModelIndex index = model->index(r, 0, parent);
        QVariant data     = model->data(index, PolicyRoles::ITEM_TYPE);

        qDebug() << "Folder " << model->data(index) << " has children: " << model->hasChildren(index)
                 << " type: " << data;

        if (model->hasChildren(index))
        {
            iterateModelAndRemoveEmptyFolders(model, index);
        }
        else
        {
            if (data == 0)
            {
                qDebug() << "Deleted folder " << model->data(index);

                model->removeRow(index.row(), index.parent());

                iterateModelAndRemoveEmptyFolders(model, index.parent());
            }
        }
    }
}

}

Q_DECLARE_METATYPE(std::shared_ptr<::security::SecurityPresentation>)
Q_DECLARE_METATYPE(std::shared_ptr<::security::SecurityDefinition>)

