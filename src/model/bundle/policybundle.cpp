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

#include "policybundle.h"

#include "../pluginstorage.h"

#include "../../plugins/admx/admxformat.h"
#include "../../plugins/adml/admlformat.h"
#include "../../io/genericfile.h"

#include "../admx/policydefinitions.h"
#include "../admx/policyelement.h"
#include "../admx/policyenumelement.h"
#include "../presentation/policyresources.h"
#include "../presentation/presentationwidget.h"
#include "../presentation/presentation.h"
#include "../presentation/dropdownlist.h"

#include "policyroles.h"

#include <QDir>
#include <QStandardItemModel>

#include <fstream>

namespace model {

namespace bundle {

struct CategoryStorage
{
    QStandardItem* machineItem;
    QStandardItem* userItem;
    model::admx::Category category;
};

struct PolicyStorage
{
    model::admx::PolicyType type;
    std::string category;
    QStandardItem* item;
};

class PolicyBundlePrivate
{
public:
    std::unique_ptr<QStandardItemModel> treeModel;
    std::map<std::string, CategoryStorage> categoryItemMap;
    std::vector<PolicyStorage> unassignedItems;
    QStandardItem* rootMachineItem;
    QStandardItem* rootUserItem;
    std::vector<QStandardItem*> items;
    std::map<std::string, std::string> supportedOnMap;
    QString languageDirectoryPath;
};

PolicyBundle::PolicyBundle()
    : d(new PolicyBundlePrivate())
{
}

PolicyBundle::~PolicyBundle()
{
    delete d;
}

std::unique_ptr<QStandardItemModel> PolicyBundle::loadFolder(const std::string& path, const std::string& language)
{
    d->treeModel = std::make_unique<QStandardItemModel>();

    QStandardItem* rootItem = d->treeModel->invisibleRootItem();

    d->rootMachineItem = createItem(QObject::tr("Machine"), "folder", QObject::tr("Machine level policies"));
    d->rootUserItem = createItem(QObject::tr("User"), "folder", QObject::tr("User level policies"));

    rootItem->appendRow(d->rootMachineItem);
    rootItem->appendRow(d->rootUserItem);

    const QDir dir(path.c_str());
    const QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    const QFileInfoList directories = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    const QString qLanguage = QString::fromStdString(language).toLower();

    for (const QFileInfo& subDir : directories)
    {
        if (subDir.fileName().toLower().endsWith(qLanguage))
        {
            d->languageDirectoryPath = subDir.absoluteFilePath();
            break;
        }
    }

    for (const QFileInfo& file : files) {
        if (file.fileName().toLower().endsWith(".admx")) {
            loadAdmxAndAdml(file);
        }
    }

    rearrangeTreeItems();
    assignSupportedOn();

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

    delete format;

    return policies;
}

QString PolicyBundle::constructFileName(const QFileInfo& fileName)
{
    QString admlFileName = fileName.fileName();
    admlFileName.replace(admlFileName.length() - 4, 4, "adml");
    QDir admlDir(d->languageDirectoryPath);
    for (const auto& file : admlDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
    {
         if (file.fileName().toLower().compare(admlFileName.toLower()) == 0)
         {
             return file.absoluteFilePath();
         }
    }

    return admlFileName;
}

std::string findStringById(const std::string& id, const std::unique_ptr<io::PolicyResourcesFile>& resource)
{
    if (id.length() < 10 || id.compare(0, 9, "$(string.") != 0)
    {
        return id;
    }
    std::string pureId = id.substr(9, id.length() - 10);
    for (auto& currentResource : resource->getAll())
    {
        auto search = currentResource->stringTable.find(pureId);
        if (search != currentResource->stringTable.end())
        {
            return search->second;
        }
    }

    return pureId;
}

std::shared_ptr<model::presentation::Presentation> findPresentationById(const std::string& id, const std::unique_ptr<io::PolicyResourcesFile>& resource)
{
    if (id.length() < 16 || id.compare(0, 15, "$(presentation.") != 0)
    {
        return nullptr;
    }
    std::string pureId = id.substr(15, id.length() - 16);
    for (auto& currentResource : resource->getAll())
    {
        auto search = currentResource->presentationTable.find(pureId);
        if (search != currentResource->presentationTable.end())
        {
            return search->second;
        }
    }

    return nullptr;
}

void handlePresentation(const std::shared_ptr<model::presentation::Presentation>& presentation,
                        const std::shared_ptr<model::admx::Policy>& policy,
                        const std::unique_ptr<io::PolicyResourcesFile>& policyResources)
{
    for (auto& widget : presentation->widgetsVector)
    {
        auto dropdownList = dynamic_cast<model::presentation::DropdownList*>(widget.get());
        if (dropdownList)
        {
            for (auto& item : policy->elements)
            {
                if (item->id.compare(dropdownList->refId) == 0)
                {
                    auto enumItem = dynamic_cast<model::admx::PolicyEnumElement*>(item.get());
                    if (enumItem)
                    {
                        for (auto& value : enumItem->items)
                        {
                            auto name = findStringById(value.first, policyResources);
                            dropdownList->values.push_back(name);
                        }
                    }
                }
            }
        }
    }
}

bool PolicyBundle::loadAdmxAndAdml(const QFileInfo& admxFileName)
{
    auto policyDefinitions = loadPolicies<io::PolicyDefinitionsFile, io::PolicyFileFormat<io::PolicyDefinitionsFile>>("admx", admxFileName);
    if (!policyDefinitions.get())
    {
        return false;
    }
    QString admlFileName = constructFileName(admxFileName);
    auto policyResources = loadPolicies<io::PolicyResourcesFile, io::PolicyFileFormat<io::PolicyResourcesFile>>("adml", admlFileName);
    if (!policyResources.get())
    {
        return false;
    }

    for (auto& definition : policyDefinitions->getAllPolicyDefinitions())
    {
        for (auto& category : definition->categories)
        {
            QString displayName = QString::fromStdString(findStringById(category->displayName, policyResources));
            QString explainText = QString::fromStdString(findStringById(category->explainText, policyResources));

            d->categoryItemMap[category->name].machineItem = createItem(displayName, "folder", explainText);
            d->categoryItemMap[category->name].userItem = createItem(displayName, "folder", explainText);
            d->categoryItemMap[category->name].category = *category;

            if (category->parentCategory.size() == 0)
            {
                d->rootUserItem->appendRow(d->categoryItemMap[category->name].userItem);
                d->rootMachineItem->appendRow(d->categoryItemMap[category->name].machineItem);
            }
        }

        if (definition->supportedOn)
        {
            for (auto& supportedOn : definition->supportedOn->definitions)
            {
                d->supportedOnMap[supportedOn->name] = findStringById(supportedOn->displayName, policyResources);
            }
        }

        for (auto& policy : definition->policies)
        {
            QString displayName = QString::fromStdString(findStringById(policy->displayName, policyResources));
            QString explainText = QString::fromStdString(findStringById(policy->explainText, policyResources));

            auto policyItem = createItem(displayName, "text-x-generic", explainText, 1);

            PolicyStorage container;
            container.category = policy->parentCategory;
            container.item = policyItem;
            container.type = policy->policyType;

            policyItem->setData(QString::fromStdString(policy->supportedOn), PolicyRoles::SUPPORTED_ON);

            if (policy->presentation) {
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

void model::bundle::PolicyBundle::assignParentCategory(const std::string& rawCategory, QStandardItem* machineItem, QStandardItem* userItem)
{
    std::string parentCategory = rawCategory;
    std::string::size_type position = parentCategory.find(':');
    if (position != std::string::npos)
    {
       parentCategory = parentCategory.substr(position + 1);
    }

    auto search = d->categoryItemMap.find(parentCategory);
    if (search != d->categoryItemMap.end())
    {
        if (machineItem) {
            search->second.machineItem->appendRow(machineItem);
        }
        if (userItem)
        {
            search->second.userItem->appendRow(userItem);
        }
    }
    else if (rawCategory.size() > 0)
    {
        qWarning() << "Unable to find parent category: " << rawCategory.c_str();
        if (machineItem)
        {
            d->rootMachineItem->appendRow(machineItem);
        }

        if (userItem)
        {
            d->rootUserItem->appendRow(userItem);
        }
    }
}

QStandardItem *PolicyBundle::createItem(const QString &displayName, const QString& iconName, const QString& explainText,
                                        const uint itemType)
{
    QStandardItem* categoryItem = new QStandardItem(displayName.trimmed());
    categoryItem->setIcon(QIcon::fromTheme(iconName));
    categoryItem->setFlags(categoryItem->flags() & (~Qt::ItemIsEditable));
    categoryItem->setData(explainText, PolicyRoles::EXPLAIN_TEXT);
    categoryItem->setData(itemType, PolicyRoles::ITEM_TYPE);

    d->items.push_back(categoryItem);

    return categoryItem;
}

void model::bundle::PolicyBundle::rearrangeTreeItems()
{
    for (const auto& entry : d->categoryItemMap)
    {
        assignParentCategory(entry.second.category.parentCategory, entry.second.machineItem, entry.second.userItem);
    }

    for (const auto& item : d->unassignedItems)
    {
        if (item.type == model::admx::PolicyType::User) {
            assignParentCategory(item.category, nullptr, item.item);
        }
        else if (item.type == model::admx::PolicyType::Machine)
        {
            assignParentCategory(item.category, item.item, nullptr);
        }
        else
        {
            QStandardItem* copyItem = createItem(item.item->text(), "text-x-generic",
                                                 item.item->data(PolicyRoles::EXPLAIN_TEXT).value<QString>(), 1);
            copyItem->setData(item.item->data(PolicyRoles::SUPPORTED_ON), PolicyRoles::SUPPORTED_ON);
            copyItem->setData(item.item->data(PolicyRoles::PRESENTATION), PolicyRoles::PRESENTATION);
            assignParentCategory(item.category, item.item, copyItem);
        }
    }
}

void PolicyBundle::assignSupportedOn()
{
    for (auto& item : d->items)
    {
        if (item->data(PolicyRoles::ITEM_TYPE).value<uint>() == 1)
        {
            QStringList supportedRaw = item->data(PolicyRoles::SUPPORTED_ON).value<QString>().split(':');
            QString* toFind = nullptr;
            if (supportedRaw.size() > 1)
            {
                toFind = &supportedRaw[1];
            }
            else
            {
                toFind = &supportedRaw[0];
            }

            auto search = d->supportedOnMap.find(toFind->toStdString());
            if (search != d->supportedOnMap.end())
            {
                item->setData(QString::fromStdString(search->second), PolicyRoles::SUPPORTED_ON);
            }
            else
            {
                qWarning() << "Not found support for: " << *toFind;
            }
        }
    }
}

}

}

Q_DECLARE_METATYPE(std::shared_ptr<::model::presentation::Presentation>)
Q_DECLARE_METATYPE(std::shared_ptr<::model::admx::Policy>)
