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

#include "widgetfactory.h"

#include <map>
#include <memory>

#include "datasource/datasourcewidget.h"
#include "device/devicewidget.h"
#include "drives/driveswidget.h"
#include "files/fileswidget.h"
#include "folders/folderwidget.h"
#include "folder_options/folderoptionswidget.h"
#include "folder_options/openwithwidget.h"
#include "ini/iniwidget.h"
#include "local_users_and_groups/localgroupwidget.h"
#include "local_users_and_groups/localuserwidget.h"
#include "network_options/dialupwidget.h"
#include "network_options/vpnwidget.h"
#include "network_options/vpnsecuritywidget.h"
#include "network_options/vpnoptionswidget.h"
#include "network_options/vpnnetworkingwidget.h"
#include "power_options/poweroptionswidget.h"
#include "power_options/powerplanwidget.h"
#include "power_options/powerschemewidget.h"
#include "printers/localprinterwidget.h"
#include "printers/sharedprinterwidget.h"
#include "printers/tcpprinterportwidget.h"
#include "printers/tcpprintergeneralwidget.h"
#include "registry/registrywidget.h"
#include "shares/shareswidget.h"
#include "shortcuts/shortcutswidget.h"
#include "variables/variableswidget.h"

namespace preferences
{

class WidgetFactoryPrivate
{
public:
    std::unordered_multimap<std::string, std::function<std::unique_ptr<PreferenceWidgetInterface>() > > constructors{};
};

WidgetFactory::WidgetFactory()
    : d(std::make_unique<WidgetFactoryPrivate>())
{
    registerItem("DrivesItem", [](){ return std::make_unique<DrivesWidget>(); });
    registerItem("DeviceItem", [](){ return std::make_unique<DeviceWidget>(); });
    registerItem("FilesItem", [](){ return std::make_unique<FilesWidget>(); });
    registerItem("FolderItem", [](){ return std::make_unique<FolderWidget>(); });
    registerItem("FolderOptionsItem", [](){ return std::make_unique<FolderOptionsWidget>(); });
    registerItem("OpenWithItem", [](){ return std::make_unique<OpenWithWidget>(); });
    registerItem("IniItem", [](){ return std::make_unique<IniWidget>(); });
    registerItem("RegistryItem", [](){ return std::make_unique<RegistryWidget>(); });
    registerItem("SharesItem", [](){ return std::make_unique<SharesWidget>(); });
    registerItem("ShortcutsItem", [](){ return std::make_unique<ShortcutsWidget>(); });
    registerItem("VariablesItem", [](){ return std::make_unique<VariablesWidget>(); });
    registerItem("DataSourceItem", [](){ return std::make_unique<DataSourceWidget>(); });
    registerItem("LocalGroupItem", [](){ return std::make_unique<LocalGroupWidget>(); });
    registerItem("LocalUserItem", [](){ return std::make_unique<LocalUserWidget>(); });
    registerItem("VpnItem", [](){ return std::make_unique<VpnWidget>(); });
    registerItem("VpnItem", [](){ return std::make_unique<VpnSecurityWidget>(); });
    registerItem("VpnItem", [](){ return std::make_unique<VpnOptionsWidget>(); });
    registerItem("VpnItem", [](){ return std::make_unique<VpnNetworkingWidget>(); });
    registerItem("DialUpItem", [](){ return std::make_unique<DialUpWidget>(); });
    registerItem("PowerOptionsItem", [](){ return std::make_unique<PowerOptionsWidget>(); });
    registerItem("PowerPlanItem", [](){ return std::make_unique<PowerPlanWidget>(); });
    registerItem("PowerSchemeItem", [](){ return std::make_unique<PowerSchemeWidget>(); });
    registerItem("LocalPrinterItem", [](){ return std::make_unique<LocalPrinterWidget>(); });
    registerItem("TcpPrinterItem", [](){ return std::make_unique<TcpPrinterGeneralWidget>(); });
    registerItem("TcpPrinterItem", [](){ return std::make_unique<TcpPrinterPortWidget>(); });
    registerItem("SharedPrinterItem", [](){ return std::make_unique<SharedPrinterWidget>(); });
}

WidgetFactory::~WidgetFactory() = default;

void WidgetFactory::registerItem(const std::string &type,
                                 std::function<std::unique_ptr<PreferenceWidgetInterface>() > func)
{
    d->constructors.insert(std::pair(type, func));
}

std::vector<std::unique_ptr<PreferenceWidgetInterface>> WidgetFactory::create(const std::string &type) const
{
    auto func = d->constructors.equal_range(type);

    std::vector<std::unique_ptr<PreferenceWidgetInterface>> widgets;

    std::transform(func.first, func.second, std::back_inserter(widgets),
                   [](const auto& element)
                   {
                        return element.second();
                   });

    return widgets;
}

}
