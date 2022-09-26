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

#include "networkmodelbuilder.h"

#include "networkcontaineritem.h"
#include "dialupitem.h"
#include "vpnitem.h"

#include "common/commonitem.h"

namespace preferences
{

NetworkModelBuilder::NetworkModelBuilder()
    : BaseModelBuilder()
{
}

std::unique_ptr<PreferencesModel> NetworkModelBuilder::schemaToModel(std::unique_ptr<NetworkOptions> &network)
{
    auto model = std::make_unique<PreferencesModel>();

    for (const auto& networkoptionsschema : network->VPN())
    {
        auto sessionItem = model->insertItem<NetworkContainerItem<VpnItem>>(model->rootItem());

        for (const auto& properties: networkoptionsschema.Properties())
        {
            auto vpn = sessionItem->getNetwork();
            auto action = getActionCheckboxState(getOptionalPropertyData(properties.action()).c_str());
            vpn->setProperty<std::string>(VpnItem::propertyToString(VpnItem::ACTION), action);
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::USER), properties.user());
            vpn->setProperty<std::string>(VpnItem::propertyToString(VpnItem::NAME), properties.name());
            vpn->setProperty<std::string>(VpnItem::propertyToString(VpnItem::IP_ADDRESS), getOptionalPropertyData(properties.ipAddress()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::USE_DNS), getOptionalPropertyData(properties.useDNS()));
            vpn->setProperty<std::string>(VpnItem::propertyToString(VpnItem::DIAL_FIRST), getOptionalPropertyData(properties.dialFirst()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::TRAY_ICON), getOptionalPropertyData(properties.trayIcon()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::SHOW_PROGRESS), getOptionalPropertyData(properties.showProgress()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::SHOW_PASSWORD), getOptionalPropertyData(properties.showPassword()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::SHOW_DOMAIN), getOptionalPropertyData(properties.showDomain()));
            vpn->setProperty<int>(VpnItem::propertyToString(VpnItem::REDIAL_COUNT), getOptionalPropertyData(properties.redialCount()));
            vpn->setProperty<int>(VpnItem::propertyToString(VpnItem::REDIAL_PAUSE), std::atoi(getOptionalPropertyData(properties.redialPause()).c_str()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::IDLE_DISCONNECT), getOptionalPropertyData(properties.idleDisconnect()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::RECONNECT), getOptionalPropertyData(properties.reconnect()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::CUSTOM_SETTINGS), getOptionalPropertyData(properties.customSettings()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::SECURE_PASSWORD), getOptionalPropertyData(properties.securePassword()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::SECURE_DATA), getOptionalPropertyData(properties.secureData()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::USE_LOGON), getOptionalPropertyData(properties.useLogon()));
            vpn->setProperty<std::string>(VpnItem::propertyToString(VpnItem::VPN_STRATEGY), getOptionalPropertyData(properties.vpnStrategy()));
            vpn->setProperty<std::string>(VpnItem::propertyToString(VpnItem::ENCRYPTION_TYPE), getOptionalPropertyData(properties.encryptionType()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::EAP), getOptionalPropertyData(properties.eap()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::PAP), getOptionalPropertyData(properties.pap()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::SPAP), getOptionalPropertyData(properties.spap()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::MS_CHAP), getOptionalPropertyData(properties.msChap()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::OLD_MS_CHAP), getOptionalPropertyData(properties.oldMsChap()));
            vpn->setProperty<bool>(VpnItem::propertyToString(VpnItem::MS_CHAP_V2), getOptionalPropertyData(properties.msChapV2()));

            auto common = sessionItem->getCommon();
            setCommonItemData(common, networkoptionsschema);
        }
    }
    for (const auto& networkoptionsschema : network->DUN())
    {
        auto sessionItem = model->insertItem<NetworkContainerItem<DialUpItem>>(model->rootItem());

        for (const auto& properties: networkoptionsschema.Properties())
        {
            auto dun = sessionItem->getNetwork();
            auto action = getActionCheckboxState(getOptionalPropertyData(properties.action()).c_str());

            dun->setProperty<std::string>(DialUpItem::propertyToString(DialUpItem::ACTION), action);
            dun->setProperty<bool>(DialUpItem::propertyToString(DialUpItem::USER), properties.user());
            dun->setProperty<bool>(DialUpItem::propertyToString(DialUpItem::SYSTEM), !properties.user());
            dun->setProperty<std::string>(DialUpItem::propertyToString(DialUpItem::NAME), properties.name());
            dun->setProperty<std::string>(DialUpItem::propertyToString(DialUpItem::PHONE_NUMBER), properties.phoneNumber());

            auto common = sessionItem->getCommon();
            setCommonItemData(common, networkoptionsschema);
        }

    }
    return model;
}

std::unique_ptr<NetworkOptions> NetworkModelBuilder::modelToSchema(std::unique_ptr<PreferencesModel> &model)
{
        auto networks = std::make_unique<NetworkOptions>("{09686AD1-5D80-48ee-A940-690A6DF02A90}");

        for (const auto& containerItem : model->topItems())
        {
            if (auto vpnContainer = dynamic_cast<NetworkContainerItem<VpnItem>*>(containerItem); vpnContainer)
            {
                auto vpnModel = vpnContainer->getNetwork();
                auto commonModel = vpnContainer->getCommon();
                auto vpn = createRootElement<VPN_t>("{0532F359-3205-4d32-ADB7-9AEC6402BECF}");

                auto properties = VPNProperties_t(vpnModel->property<bool>(VpnItem::propertyToString(VpnItem::USER)),
                                                  vpnModel->property<std::string>(VpnItem::propertyToString(VpnItem::NAME)));
                properties.ipAddress(vpnModel->propertyToString(VpnItem::IP_ADDRESS));
                properties.useDNS(vpnModel->propertyToString(VpnItem::USE_DNS));
                properties.dialFirst(vpnModel->propertyToString(VpnItem::DIAL_FIRST));
                properties.trayIcon(vpnModel->propertyToString(VpnItem::TRAY_ICON));
                properties.showProgress(vpnModel->propertyToString(VpnItem::SHOW_PROGRESS));
                properties.showPassword(vpnModel->propertyToString(VpnItem::SHOW_PASSWORD));
                properties.showDomain(vpnModel->propertyToString(VpnItem::SHOW_DOMAIN));
                properties.redialCount(vpnModel->propertyToInt(VpnItem::REDIAL_COUNT));
                properties.redialPause(vpnModel->propertyToString(VpnItem::REDIAL_PAUSE));
                properties.idleDisconnect(vpnModel->propertyToString(VpnItem::IDLE_DISCONNECT));
                properties.reconnect(vpnModel->propertyToString(VpnItem::RECONNECT));
                properties.customSettings(vpnModel->propertyToString(VpnItem::CUSTOM_SETTINGS));
                properties.securePassword(vpnModel->propertyToString(VpnItem::SECURE_PASSWORD));
                properties.secureData(vpnModel->propertyToString(VpnItem::SECURE_DATA));
                properties.useLogon(vpnModel->propertyToString(VpnItem::USE_LOGON));
                properties.vpnStrategy(vpnModel->propertyToString(VpnItem::VPN_STRATEGY));
                properties.encryptionType(vpnModel->propertyToString(VpnItem::ENCRYPTION_TYPE));
                properties.eap(vpnModel->propertyToString(VpnItem::EAP));
                properties.pap(vpnModel->propertyToString(VpnItem::PAP));
                properties.spap(vpnModel->propertyToString(VpnItem::SPAP));
                properties.msChap(vpnModel->propertyToString(VpnItem::MS_CHAP));
                properties.msChapV2(vpnModel->propertyToString(VpnItem::MS_CHAP_V2));
                properties.oldMsChap(vpnModel->propertyToString(VpnItem::OLD_MS_CHAP));

                setCommonModelData(vpn, commonModel);
                vpn.Properties().push_back(properties);
            }
            if (auto dunContainer = dynamic_cast<NetworkContainerItem<DialUpItem>*>(containerItem); dunContainer)
            {
                auto dunModel = dunContainer->getNetwork();
                auto commonModel = dunContainer->getCommon();
                auto dun = createRootElement<DUN_t>("{9B0D030D-9396-49c1-8DEF-08B35B5BB79E}");

                auto properties = DUNProperties_t(dunModel->property<bool>(DialUpItem::propertyToString(DialUpItem::USER)),
                                                  dunModel->property<std::string>(DialUpItem::propertyToString(DialUpItem::NAME)),
                                                  dunModel->property<std::string>(DialUpItem::propertyToString(DialUpItem::PHONE_NUMBER)));


                setCommonModelData(dun, commonModel);
                dun.Properties().push_back(properties);

            }
        }
        return networks;
}
}

