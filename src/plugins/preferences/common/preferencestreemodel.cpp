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
    static inline const std::string HELP_MSG  = "HELP_MSG";

public:
    FolderItem()
        : ModelView::GroupItem("FolderGroupItem")
    {
        addProperty(NODE_ID, QUuid::createUuid());
        addProperty(PARENT_ID, QUuid::createUuid());
        addProperty(HELP_MSG, "");
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

    auto topItem = insertItem<FolderItem>(this->rootItem());
    topItem->setDisplayName(QObject::tr("[Local Group Policy]").toStdString());
    topItem->setProperty(FolderItem::HELP_MSG, "");
    auto topUuid = QUuid("{123e4567-e89b-12d3-a456-426652340003}");
    topItem->setProperty(FolderItem::NODE_ID, topUuid);
    topItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Group policiy").toStdString());

    auto machineNamespace = insertItem<FolderItem>(topItem);
    machineNamespace->setDisplayName(QObject::tr("Machine").toStdString());
    auto machineUuid = QUuid("{123e4567-e89b-12d3-a456-426652340000}");
    machineNamespace->setProperty(FolderItem::NODE_ID, machineUuid);
    machineNamespace->setProperty(FolderItem::PARENT_ID, topUuid);
    machineNamespace->setProperty(FolderItem::HELP_MSG, QObject::tr("Machine level policies").toStdString());

    auto machinePreferencesItem = insertItem<FolderItem>(machineNamespace);
    machinePreferencesItem->setDisplayName(QObject::tr("Preferences").toStdString());
    auto machinePreferencesUuid = QUuid::createUuid();
    machinePreferencesItem->setProperty(FolderItem::NODE_ID, machinePreferencesUuid);
    machinePreferencesItem->setProperty(FolderItem::PARENT_ID, machineUuid);
    machinePreferencesItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Preferences policies.").toStdString());

    auto machineSystemSettingsItem = insertItem<FolderItem>(machinePreferencesItem);
    machineSystemSettingsItem->setDisplayName(QObject::tr("System Settings").toStdString());
    auto machineSystemSettingsUuid = QUuid::createUuid();
    machineSystemSettingsItem->setProperty(FolderItem::NODE_ID, machineSystemSettingsUuid);
    machineSystemSettingsItem->setProperty(FolderItem::PARENT_ID, machinePreferencesUuid);
    machineSystemSettingsItem->setProperty(FolderItem::HELP_MSG,
                                           QObject::tr("Policies that set system settings.").toStdString());

    auto machineVariablesItems = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineVariablesItems->setDisplayName(QObject::tr("Environment").toStdString());
    machineVariablesItems->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, variablesTypeIds);
    machineVariablesItems->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);
    machineVariablesItems->setProperty(FolderItem::HELP_MSG,
                                       QObject::tr("Environment variable settings.").toStdString());

    auto machineFilesItem = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineFilesItem->setDisplayName(QObject::tr("Files").toStdString());
    machineFilesItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, filesTypeIds);
    machineFilesItem->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);
    machineFilesItem->setProperty(FolderItem::HELP_MSG, QObject::tr("File policy settings.").toStdString());

    auto machineFoldersItem = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineFoldersItem->setDisplayName(QObject::tr("Folders").toStdString());
    machineFoldersItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, foldersTypeIds);
    machineFoldersItem->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);
    machineFoldersItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Folder policy settings.").toStdString());

    auto machineIniItem = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineIniItem->setDisplayName(QObject::tr("Ini Files").toStdString());
    machineIniItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, iniTypeIds);
    machineIniItem->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);
    machineIniItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Ini policy settings.").toStdString());

    auto machineRegistryItem = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineRegistryItem->setDisplayName(QObject::tr("Registry").toStdString());
    machineRegistryItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, registryTypeIds);
    machineRegistryItem->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);
    machineRegistryItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Registry policy settings.").toStdString());

    auto machineSharesItem = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineSharesItem->setDisplayName(QObject::tr("Network Shares").toStdString());
    machineSharesItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, sharesTypeIds);
    machineSharesItem->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);
    machineSharesItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Network shares settings.").toStdString());

    auto machineShortcutsItems = insertItem<PreferenceCategoryItem>(machineSystemSettingsItem);
    machineShortcutsItems->setDisplayName(QObject::tr("Shortcuts").toStdString());
    machineShortcutsItems->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, shortcutsTypeIds);
    machineShortcutsItems->setProperty(FolderItem::PARENT_ID, machineSystemSettingsUuid);
    machineShortcutsItems->setProperty(FolderItem::HELP_MSG, QObject::tr("Shortcuts settings.").toStdString());

    auto machineControlPanelSettingsItem = insertItem<FolderItem>(machinePreferencesItem);
    machineControlPanelSettingsItem->setDisplayName(QObject::tr("Control Panel Settings").toStdString());
    machineControlPanelSettingsItem->setProperty(FolderItem::HELP_MSG,
                                                 QObject::tr("Control Panel Settings.").toStdString());

    auto machineDataSourceItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machineDataSourceItem->setDisplayName(QObject::tr("Data Sources").toStdString());
    machineDataSourceItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, dataSourceTypeIds);
    machineDataSourceItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Data Source settings.").toStdString());

    auto machineDeviceItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machineDeviceItem->setDisplayName(QObject::tr("Devices").toStdString());
    machineDeviceItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, deviceTypeIds);
    machineDeviceItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Devices settings.").toStdString());

    auto machineFolderOptionsItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machineFolderOptionsItem->setDisplayName(QObject::tr("Folder Options").toStdString());
    machineFolderOptionsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, {});
    machineFolderOptionsItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Folders settings.").toStdString());

    auto machineLocalUsersAndGroupsItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machineLocalUsersAndGroupsItem->setDisplayName("Local Users And Groups");
    machineLocalUsersAndGroupsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE,
                                                                                localGroupTypeIds);
    machineLocalUsersAndGroupsItem->setProperty(FolderItem::HELP_MSG,
                                                QObject::tr("Local Users And Groups settings.").toStdString());

    auto machineNetworkOptionsItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machineNetworkOptionsItem->setDisplayName(QObject::tr("Network Options").toStdString());
    machineNetworkOptionsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, networkTypeIds);
    machineNetworkOptionsItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Network Options settings.").toStdString());

    auto machinePowerOptionsItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machinePowerOptionsItem->setDisplayName(QObject::tr("Power Options").toStdString());
    machinePowerOptionsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, powerTypeIds);
    machinePowerOptionsItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Power settings.").toStdString());

    auto machinePrintersItem = insertItem<PreferenceCategoryItem>(machineControlPanelSettingsItem);
    machinePrintersItem->setDisplayName(QObject::tr("Printers").toStdString());
    machinePrintersItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, printerTypeIds);
    machinePrintersItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Printers settings.").toStdString());

    //======================================================================================================================

    auto userNamespace = insertItem<FolderItem>(topItem);
    userNamespace->setDisplayName(QObject::tr("User").toStdString());
    auto userUuid = QUuid("{123e4567-e89b-12d3-a456-426652340001}");
    userNamespace->setProperty(FolderItem::NODE_ID, userUuid);
    userNamespace->setProperty(FolderItem::PARENT_ID, topUuid);
    userNamespace->setProperty(FolderItem::HELP_MSG, QObject::tr("User level policies.").toStdString());

    auto userPreferencesItem = insertItem<FolderItem>(userNamespace);
    userPreferencesItem->setDisplayName(QObject::tr("Preferences").toStdString());
    userPreferencesItem->setProperty(FolderItem::PARENT_ID, userUuid);
    userPreferencesItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Preferences policies.").toStdString());

    auto userSystemSettingsItem = insertItem<FolderItem>(userPreferencesItem);
    userSystemSettingsItem->setDisplayName(QObject::tr("System Settings").toStdString());
    userSystemSettingsItem->setProperty(FolderItem::HELP_MSG,
                                        QObject::tr("Policies that set system settings.").toStdString());

    auto userDrivesItem = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userDrivesItem->setDisplayName(QObject::tr("Drive Maps").toStdString());
    userDrivesItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, drivesTypeIds);
    userDrivesItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Drive Maps Settings.").toStdString());

    auto userVariablesItems = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userVariablesItems->setDisplayName(QObject::tr("Environment").toStdString());
    userVariablesItems->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, variablesTypeIds);
    userVariablesItems->setProperty(FolderItem::HELP_MSG, QObject::tr("Environment variable settings.").toStdString());

    auto userFilesItem = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userFilesItem->setDisplayName(QObject::tr("Files").toStdString());
    userFilesItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, filesTypeIds);
    userFilesItem->setProperty(FolderItem::HELP_MSG, QObject::tr("File policy settings.").toStdString());

    auto userFoldersItem = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userFoldersItem->setDisplayName(QObject::tr("Folders").toStdString());
    userFoldersItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, foldersTypeIds);
    userFoldersItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Folders policy settings.").toStdString());

    auto userIniItem = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userIniItem->setDisplayName(QObject::tr("Ini Files").toStdString());
    userIniItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, iniTypeIds);
    userIniItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Ini policy settings.").toStdString());

    auto userRegistryItem = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userRegistryItem->setDisplayName(QObject::tr("Registry").toStdString());
    userRegistryItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, registryTypeIds);
    userRegistryItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Registry policy settings.").toStdString());

    auto userShortcutsItems = insertItem<PreferenceCategoryItem>(userSystemSettingsItem);
    userShortcutsItems->setDisplayName(QObject::tr("Shortcuts").toStdString());
    userShortcutsItems->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, shortcutsTypeIds);
    userShortcutsItems->setProperty(FolderItem::HELP_MSG, QObject::tr("Shortcuts settings.").toStdString());

    auto userControlPanelSettingsItem = insertItem<FolderItem>(userPreferencesItem);
    userControlPanelSettingsItem->setDisplayName(QObject::tr("Control Panel Settings").toStdString());
    userControlPanelSettingsItem->setProperty(FolderItem::HELP_MSG,
                                              QObject::tr("Control Panel Settings.").toStdString());

    auto userDataSourceItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userDataSourceItem->setDisplayName(QObject::tr("Data Sources").toStdString());
    userDataSourceItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, dataSourceTypeIds);
    userDataSourceItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Data Source settings.").toStdString());

    auto userDeviceItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userDeviceItem->setDisplayName(QObject::tr("Devices").toStdString());
    userDeviceItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, deviceTypeIds);
    userDeviceItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Devices settings.").toStdString());

    auto userFolderOptionsItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userFolderOptionsItem->setDisplayName(QObject::tr("Folder Options").toStdString());
    userFolderOptionsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE,
                                                                       userFolderOptionsTypeIds);
    userFolderOptionsItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Folders settings.").toStdString());

    auto userLocalUsersAndGroupsItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userLocalUsersAndGroupsItem->setDisplayName(QObject::tr("Local Users And Groups").toStdString());
    userLocalUsersAndGroupsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE,
                                                                             localGroupTypeIds);
    userLocalUsersAndGroupsItem->setProperty(FolderItem::HELP_MSG,
                                             QObject::tr("Local Users And Groups settings.").toStdString());

    auto userNetworkOptionsItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userNetworkOptionsItem->setDisplayName(QObject::tr("Network Options").toStdString());
    userNetworkOptionsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, networkTypeIds);
    userNetworkOptionsItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Network Options settings.").toStdString());

    auto userPowerOptionsItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userPowerOptionsItem->setDisplayName(QObject::tr("Power Options").toStdString());
    userPowerOptionsItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, powerTypeIds);
    userPowerOptionsItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Power settings.").toStdString());

    auto userPrintersItem = insertItem<PreferenceCategoryItem>(userControlPanelSettingsItem);
    userPrintersItem->setDisplayName(QObject::tr("Printers").toStdString());
    userPrintersItem->setProperty<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE, printerTypeIds);
    userPrintersItem->setProperty(FolderItem::HELP_MSG, QObject::tr("Printers settings.").toStdString());
}

} // namespace preferences
