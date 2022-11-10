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

#include "preferencesmodel.h"

#include "datasource/datasourcecontaineritem.h"
#include "device/devicecontaineritem.h"
#include "drives/drivescontaineritem.h"
#include "files/filescontaineritem.h"
#include "folders/foldercontaineritem.h"
#include "folder_options/folderoptionscontaineritem.h"
#include "folder_options/folderoptionsitem.h"
#include "folder_options/openwithitem.h"
#include "ini/inicontaineritem.h"
#include "local_users_and_groups/localgroupcontaineritem.h"
#include "local_users_and_groups/localgroupitem.h"
#include "local_users_and_groups/localuseritem.h"
#include "local_users_and_groups/groupmemberitem.h"
#include "local_users_and_groups/groupmemberscontaineritem.h"
#include "network_options/networkcontaineritem.h"
#include "network_options/dialupitem.h"
#include "network_options/vpnitem.h"
#include "power_options/powercontaineritem.h"
#include "power_options/poweroptionsitem.h"
#include "power_options/powerplanitem.h"
#include "power_options/powerschemeitem.h"
#include "registry/registrycontaineritem.h"
#include "printers/printercontaineritem.h"
#include "printers/localprinteritem.h"
#include "printers/sharedprinteritem.h"
#include "printers/tcpprinteritem.h"
#include "shares/sharescontaineritem.h"
#include "shortcuts/shortcutscontaineritem.h"
#include "variables/variablescontaineritem.h"

namespace preferences
{

PreferencesModel::PreferencesModel()
    : ::ModelView::SessionModel("PreferencesModel")
{
    registerItem<DataSourceContainerItem>();
    registerItem<DrivesContainerItem>();
    registerItem<DeviceContainerItem>();
    registerItem<FilesContainerItem>();
    registerItem<FolderContainerItem>();
    registerItem<FolderOptionsContainerItem<FolderOptionsItem>>();
    registerItem<FolderOptionsContainerItem<OpenWithItem>>();
    registerItem<IniContainerItem>();
    registerItem<RegistryContainerItem>();
    registerItem<SharesContainerItem>();
    registerItem<ShortcutsContainerItem>();
    registerItem<VariablesContainerItem>();
    registerItem<LocalGroupContainerItem<LocalGroupItem>>();
    registerItem<LocalGroupContainerItem<LocalUserItem>>();
    registerItem<GroupMemberItem>();
    registerItem<GroupMembersContainerItem>();
    registerItem<NetworkContainerItem<DialUpItem>>();
    registerItem<NetworkContainerItem<VpnItem>>();
    registerItem<PowerContainerItem<PowerOptionsItem>>();
    registerItem<PowerContainerItem<PowerPlanItem>>();
    registerItem<PowerContainerItem<PowerSchemeItem>>();
    registerItem<PrinterContainerItem<LocalPrinterItem>>();
    registerItem<PrinterContainerItem<SharedPrinterItem>>();
    registerItem<PrinterContainerItem<TcpPrinterItem>>();
}

}
