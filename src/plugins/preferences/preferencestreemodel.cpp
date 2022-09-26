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

#include "preferencestreemodel.h"

#include "common/preferencecategoryitem.h"

#include <mvvm/model/groupitem.h>
#include <mvvm/model/propertyitem.h>
#include <mvvm/model/taginfo.h>

#include "folder_options/folderoptionscontaineritem.h"
#include "folder_options/folderoptionsitem.h"
#include "folder_options/openwithitem.h"

#include "local_users_and_groups/localgroupcontaineritem.h"
#include "local_users_and_groups/localgroupitem.h"
#include "local_users_and_groups/localuseritem.h"

#include "network_options/dialupitem.h"
#include "network_options/networkcontaineritem.h"
#include "network_options/vpnitem.h"

#include "power_options/powercontaineritem.h"
#include "power_options/poweroptionsitem.h"
#include "power_options/powerschemeitem.h"

#include "printers/localprinteritem.h"
#include "printers/printercontaineritem.h"
#include "printers/sharedprinterwidget.h"
#include "printers/tcpprinteritem.h"

namespace
{
class FolderItem : public ModelView::GroupItem
{
public:
    static inline const std::string NODE_ID   = "NODE_ID";
    static inline const std::string PARENT_ID = "PARENT_ID";

public:
    FolderItem()
        : ModelView::GroupItem("FolderGroupItem")
    {
        addProperty(NODE_ID, QUuid::createUuid());
        addProperty(PARENT_ID, QUuid::createUuid());
    }

    template<typename T>
    T *addProperty(const std::string &name)
    {
        registerTag(ModelView::TagInfo::propertyTag(name, T().modelType()));
        auto result = insertItem<T>({name, 0});
        result->setDisplayName(name);
        return result;
    }

    inline ModelView::PropertyItem *addProperty(const std::string &name, const char *value)
    {
        return addProperty(name, std::string(value));
    }

    template<typename V>
    ModelView::PropertyItem *addProperty(const std::string &name, const V &value)
    {
        auto property = addProperty<ModelView::PropertyItem>(name);
        property->setData(value);
        if constexpr (std::is_floating_point_v<V>)
            property->setData(ModelView::RealLimits::limitless(), ModelView::ItemDataRole::LIMITS);
        return property;
    }
};

} // namespace

namespace preferences
{
PreferencesTreeModel::PreferencesTreeModel()
    : ModelView::SessionModel("PreferencesTreeModel")
{
    registerItem<FolderItem>();
    registerItem<PreferenceCategoryItem>();

    populateModel();
}

void PreferencesTreeModel::populateModel()
{
    std::map<std::string, QString> drivesTypeIds{{"DrivesContainerItem", QObject::tr("Mapped Drive")}};

    std::map<std::string, QString> variablesTypeIds{{"VariablesContainerItem", QObject::tr("Environment Variable")}};

    std::map<std::string, QString> filesTypeIds{{"FilesContainerItem", QObject::tr("File")}};

    std::map<std::string, QString> foldersTypeIds{{"FolderContainerItem", QObject::tr("Folder")}};

    std::map<std::string, QString> iniTypeIds{{"IniContainerItem", QObject::tr("Ini File")}};

    std::map<std::string, QString> registryTypeIds{{"RegistryContainerItem", QObject::tr("Registry Value")}};

    std::map<std::string, QString> sharesTypeIds{{"SharesContainerItem", QObject::tr("Network Share")}};

    std::map<std::string, QString> shortcutsTypeIds{{"ShortcutsContainerItem", QObject::tr("Shortcut")}};

    std::map<std::string, QString> dataSourceTypeIds{{"DataSourceContainerItem", QObject::tr("Data Source")}};

    std::map<std::string, QString> deviceTypeIds{{"DeviceContainerItem", QObject::tr("Device")}};

    std::map<std::string, QString> localGroupTypeIds{{typeid(LocalGroupContainerItem<LocalGroupItem>).name(),
                                                      QObject::tr("Local Group")},
                                                     {typeid(LocalGroupContainerItem<LocalUserItem>).name(),
                                                      QObject::tr("Local User")}};

    std::map<std::string, QString> networkTypeIds{{typeid(NetworkContainerItem<VpnItem>).name(),
                                                   QObject::tr("VPN Connection")},
                                                  {typeid(NetworkContainerItem<DialUpItem>).name(),
                                                   QObject::tr("Dial-Up Connection")}};

    std::map<std::string, QString> powerTypeIds{{typeid(PowerContainerItem<PowerOptionsItem>).name(),
                                                 QObject::tr("Power Options Settings")},
                                                {typeid(PowerContainerItem<PowerSchemeItem>).name(),
                                                 QObject::tr("Power Scheme Settings")}};

    std::map<std::string, QString> printerTypeIds{{typeid(PrinterContainerItem<SharedPrinterItem>).name(),
                                                   QObject::tr("Shared Printer")},
                                                  {typeid(PrinterContainerItem<TcpPrinterItem>).name(),
                                                   QObject::tr("TCP/IP Printer")},
                                                  {typeid(PrinterContainerItem<LocalPrinterItem>).name(),
                                                   QObject::tr("Local Printer")}};

    std::map<std::string, QString> userFolderOptionsTypeIds{{typeid(FolderOptionsContainerItem<FolderOptionsItem>).name(),
                                                             QObject::tr("Folder Options")},
                                                            {typeid(FolderOptionsContainerItem<OpenWithItem>).name(),
                                                             QObject::tr("Open With")}};

    auto machineNamespace = insertItem<FolderItem>(this->rootItem());
    machineNamespace->setDisplayName("Machine");
    auto machineUuid = QUuid("{123e4567-e89b-12d3-a456-426652340000}");
    machineNamespace->setProperty(FolderItem::NODE_ID, machineUuid);

    auto machinePreferencesItem = insertItem<FolderItem>(machineNamespace);
    machinePreferencesItem->setDisplayName("Preferences");
    auto machinePreferencesUuid = QUuid::createUuid();
    machinePreferencesItem->setProperty(FolderItem::NODE_ID, machinePreferencesUuid);
    machinePreferencesItem->setProperty(FolderItem::PARENT_ID, machineUuid);

    auto machineSystemSettingsItem = insertItem<FolderItem>(machinePreferencesItem);
    machineSystemSettingsItem->setDisplayName("System Settings");
    auto machineSystemSettingsUuid = QUuid::createUuid();
    machineSystemSettingsItem->setProperty(FolderItem::NODE_ID, machineSystemSettingsUuid);
    machineSystemSettingsItem->setProperty(FolderItem::PARENT_ID, machinePreferencesUuid);

    auto machineVariablesItems = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineVariablesItems->setDisplayName("Environment");
    machineVariablesItems->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, variablesTypeIds);
    machineVariablesItems->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);

    auto machineFilesItem = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineFilesItem->setDisplayName("Files");
    machineFilesItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, filesTypeIds);
    machineFilesItem->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);

    auto machineFoldersItem = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineFoldersItem->setDisplayName("Folders");
    machineFoldersItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, foldersTypeIds);
    machineFoldersItem->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);

    auto machineIniItem = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineIniItem->setDisplayName("Ini Files");
    machineIniItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, iniTypeIds);
    machineIniItem->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);

    auto machineRegistryItem = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineRegistryItem->setDisplayName("Registry");
    machineRegistryItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, registryTypeIds);
    machineRegistryItem->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);

    auto machineSharesItem = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineSharesItem->setDisplayName("Network Shares");
    machineSharesItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, sharesTypeIds);
    machineSharesItem->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);

    auto machineShortcutsItems = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineShortcutsItems->setDisplayName("Shortcuts");
    machineShortcutsItems->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, shortcutsTypeIds);
    machineShortcutsItems->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);

    auto machineControlPanelSettingsItem = insertItem<FolderItem>(machinePreferencesItem);
    machineControlPanelSettingsItem->setDisplayName("Control Panel Settings");

    auto machineDataSourceItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machineDataSourceItem->setDisplayName("Data Sources");
    machineDataSourceItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, dataSourceTypeIds);

    auto machineDeviceItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machineDeviceItem->setDisplayName("Devices");
    machineDeviceItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, deviceTypeIds);

    auto machineFolderOptionsItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machineFolderOptionsItem->setDisplayName("Folder Options");
    machineFolderOptionsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, {});

    auto machineLocalUsersAndGroupsItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machineLocalUsersAndGroupsItem->setDisplayName("Local Users And Groups");
    machineLocalUsersAndGroupsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE,
                                                                                localGroupTypeIds);

    auto machineNetworkOptionsItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machineNetworkOptionsItem->setDisplayName("Network Options");
    machineNetworkOptionsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, networkTypeIds);

    auto machinePowerOptionsItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machinePowerOptionsItem->setDisplayName("Power Options");
    machinePowerOptionsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, powerTypeIds);

    auto machinePrintersItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machinePrintersItem->setDisplayName("Printers");
    machinePrintersItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, printerTypeIds);

    //======================================================================================================================

    auto userNamespace = insertItem<FolderItem>(this->rootItem());
    userNamespace->setDisplayName("User");
    auto userUuid = QUuid("{123e4567-e89b-12d3-a456-426652340001}");
    userNamespace->setProperty(FolderItem::NODE_ID, userUuid);

    auto userPreferencesItem = insertItem<FolderItem>(userNamespace);
    userPreferencesItem->setDisplayName("Preferences");
    userPreferencesItem->setProperty(FolderItem::PARENT_ID, userUuid);

    auto userSystemSettingsItem = insertItem<FolderItem>(userPreferencesItem);
    userSystemSettingsItem->setDisplayName("System Settings");

    auto userDrivesItem = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userDrivesItem->setDisplayName("Drive Maps");
    userDrivesItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, drivesTypeIds);

    auto userVariablesItems = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userVariablesItems->setDisplayName("Environment");
    userVariablesItems->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, variablesTypeIds);

    auto userFilesItem = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userFilesItem->setDisplayName("Files");
    userFilesItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, filesTypeIds);

    auto userFoldersItem = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userFoldersItem->setDisplayName("Folders");
    userFoldersItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, foldersTypeIds);

    auto userIniItem = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userIniItem->setDisplayName("Ini Files");
    userIniItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, iniTypeIds);

    auto userRegistryItem = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userRegistryItem->setDisplayName("Registry");
    userRegistryItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, registryTypeIds);

    auto userShortcutsItems = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userShortcutsItems->setDisplayName("Shortcuts");
    userShortcutsItems->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, shortcutsTypeIds);

    auto userControlPanelSettingsItem = insertItem<FolderItem>(userPreferencesItem);
    userControlPanelSettingsItem->setDisplayName("Control Panel Settings");

    auto userDataSourceItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userDataSourceItem->setDisplayName("Data Sources");
    userDataSourceItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, dataSourceTypeIds);

    auto userDeviceItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userDeviceItem->setDisplayName("Devices");
    userDeviceItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, deviceTypeIds);

    auto userFolderOptionsItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userFolderOptionsItem->setDisplayName("Folder Options");
    userFolderOptionsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE,
                                                                       userFolderOptionsTypeIds);

    auto userLocalUsersAndGroupsItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userLocalUsersAndGroupsItem->setDisplayName("Local Users And Groups");
    userLocalUsersAndGroupsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE,
                                                                             localGroupTypeIds);

    auto userNetworkOptionsItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userNetworkOptionsItem->setDisplayName("Network Options");
    userNetworkOptionsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, networkTypeIds);

    auto userPowerOptionsItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userPowerOptionsItem->setDisplayName("Power Options");
    userPowerOptionsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, powerTypeIds);

    auto userPrintersItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userPrintersItem->setDisplayName("Printers");
    userPrintersItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, printerTypeIds);
}

} // namespace preferences
