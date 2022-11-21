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

#include "printersmodelbuilder.h"

#include "printercontaineritem.h"
#include "localprinteritem.h"
#include "sharedprinteritem.h"
#include "tcpprinteritem.h"

#include "common/commonitem.h"

namespace preferences
{
PrintersModelBuilder::PrintersModelBuilder()
    : BaseModelBuilder()
{}

void PrintersModelBuilder::processLocalPrinter(LocalPrinterItem *localPrinter,
                                                            LocalPrinterProperties_t properties)
{
    std::string action = properties.action().present() ? properties.action().get() : "";

    int actionState = getActionCheckboxState(action.c_str());
    std::string name     = properties.name().c_str();
    std::string port     = properties.port().c_str();
    std::string path     = properties.path().c_str();
    bool defaultOn       = properties.default_();
    std::string location = getOptionalPropertyData(properties.location());
    std::string comment  = getOptionalPropertyData(properties.comment());
    bool deleteAll       = properties.deleteAll();

    localPrinter->setProperty(LocalPrinterItem::propertyToString(LocalPrinterItem::ACTION), actionState);
    localPrinter->setProperty(LocalPrinterItem::propertyToString(LocalPrinterItem::NAME), name);
    localPrinter->setProperty(LocalPrinterItem::propertyToString(LocalPrinterItem::PORT), port);
    localPrinter->setProperty(LocalPrinterItem::propertyToString(LocalPrinterItem::PATH), path);
    localPrinter->setProperty(LocalPrinterItem::propertyToString(LocalPrinterItem::DEFAULT), defaultOn);
    localPrinter->setProperty(LocalPrinterItem::propertyToString(LocalPrinterItem::LOCATION), location.c_str());
    localPrinter->setProperty(LocalPrinterItem::propertyToString(LocalPrinterItem::COMMENT), comment.c_str());
    localPrinter->setProperty(LocalPrinterItem::propertyToString(LocalPrinterItem::DELETE_ALL), deleteAll);
}

void PrintersModelBuilder::processSharedPrinter(SharedPrinterItem *sharedPrinter, SharedPrinterProperties_t properties)
{
    int actionState      = getActionCheckboxState(getOptionalPropertyData(properties.action()).c_str());
    std::string location = getOptionalPropertyData(properties.location()).c_str();
    std::string path     = properties.path().c_str();
    std::string comment  = getOptionalPropertyData(properties.comment()).c_str();
    bool defaultOn       = properties.default_();
    bool skipLocal       = properties.skipLocal();
    bool deleteAll       = properties.deleteAll();
    std::string port     = properties.port().c_str();
    bool persistent      = properties.persistent();
    bool deleteMaps      = properties.deleteMaps();
    std::string username = getOptionalPropertyData(properties.username()).c_str();
    std::string password = getOptionalPropertyData(properties.cpassword()).c_str();

    sharedPrinter->setProperty(SharedPrinterItem::propertyToString(SharedPrinterItem::ACTION), actionState);
    sharedPrinter->setProperty(SharedPrinterItem::propertyToString(SharedPrinterItem::LOCATION), location);
    sharedPrinter->setProperty(SharedPrinterItem::propertyToString(SharedPrinterItem::PATH), path);
    sharedPrinter->setProperty(SharedPrinterItem::propertyToString(SharedPrinterItem::COMMENT), comment);
    sharedPrinter->setProperty(SharedPrinterItem::propertyToString(SharedPrinterItem::DEFAULT), defaultOn);
    sharedPrinter->setProperty(SharedPrinterItem::propertyToString(SharedPrinterItem::SKIP_LOCAL), skipLocal);
    sharedPrinter->setProperty(SharedPrinterItem::propertyToString(SharedPrinterItem::DELETE_ALL), deleteAll);
    sharedPrinter->setProperty(SharedPrinterItem::propertyToString(SharedPrinterItem::PORT), port);
    sharedPrinter->setProperty(SharedPrinterItem::propertyToString(SharedPrinterItem::PERSISTENT), persistent);
    sharedPrinter->setProperty(SharedPrinterItem::propertyToString(SharedPrinterItem::DELETE_MAPS), deleteMaps);
    sharedPrinter->setProperty(SharedPrinterItem::propertyToString(SharedPrinterItem::USERNAME), username);
    sharedPrinter->setProperty(SharedPrinterItem::propertyToString(SharedPrinterItem::PASSWORD), password);
}

void PrintersModelBuilder::processTcpPrinter(TcpPrinterItem *tcpPrinter, PortPrinterProperties_t properties)
{
    int actionState           = getActionCheckboxState(getOptionalPropertyData(properties.action()).c_str());
    std::string ipAddress     = properties.ipAddress().c_str();
    bool useDns               = getOptionalPropertyData(properties.useDNS());
    std::string localName     = getOptionalPropertyData(properties.localName()).c_str();
    std::string path          = properties.path().c_str();
    bool defaultOn            = properties.default_();
    bool skipLocal            = properties.skipLocal();
    bool deleteAll            = properties.deleteAll();
    std::string location      = getOptionalPropertyData(properties.location()).c_str();
    std::string comment       = getOptionalPropertyData(properties.comment()).c_str();
    std::string lprQueue      = getOptionalPropertyData(properties.lprQueue()).c_str();
    std::string snmpCommunity = getOptionalPropertyData(properties.snmpCommunity()).c_str();
    int portNumber            = getOptionalPropertyData(properties.portNumber());
    bool doubleSpool          = getOptionalPropertyData(properties.doubleSpool());
    bool snmpEnabled          = getOptionalPropertyData(properties.snmpEnabled());
    int snmpDevIndex          = getOptionalPropertyData(properties.snmpDevIndex());

    std::string protocol = getOptionalPropertyData(properties.protocol()).c_str();

    bool lprProtocol = true;

    if (protocol.compare("PROTOCOL_RAWTCP_TYPE") == 0)
    {
        lprProtocol = false;
    }

    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::ACTION), actionState);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::IP_ADDRESS), ipAddress);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::USE_DNS), useDns);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::LOCAL_NAME), localName);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::PATH), path);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::DEFAULT), defaultOn);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::SKIP_LOCAL), skipLocal);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::DELETE_ALL), deleteAll);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::LOCATION), location);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::COMMENT), comment);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::LPR_QUEUE), lprQueue);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::SNMP_COMMUNITY), snmpCommunity);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::PORT_NUMBER), portNumber);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::DOUBLE_SPOOL), doubleSpool);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::SNMP_ENABLED), snmpEnabled);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::SNMP_DEVINDEX), snmpDevIndex);

    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::LPR_PROTOCOL), lprProtocol);
    tcpPrinter->setProperty(TcpPrinterItem::propertyToString(TcpPrinterItem::TCP_PROTOCOL), !lprProtocol);
}

template<typename PrinterSourceType, typename ItemType, typename PropertiesType>
void preferences::PrintersModelBuilder::processLocalPrinters(PrinterSourceType &printersSource,
                                                             std::unique_ptr<PreferencesModel> &model,
                                                             std::function<void(ItemType *, PropertiesType)> &callback)
{
    for (const auto &localPrintersSchema : printersSource)
    {
        auto sessionItem = model->insertItem<PrinterContainerItem<ItemType>>(model->rootItem());

        for (const auto &properties : localPrintersSchema.Properties())
        {
            ItemType *localPrinter = sessionItem->getPrinter();

            callback(localPrinter, properties);

            auto common = sessionItem->getCommon();
            setCommonItemData(common, localPrintersSchema);
        }
    }
}

std::unique_ptr<PreferencesModel> PrintersModelBuilder::schemaToModel(std::unique_ptr<Printers> &printersSource)
{
    auto model = std::make_unique<PreferencesModel>();

    std::function<void(LocalPrinterItem *, LocalPrinterProperties_t)> localPrinterHandler =
        [&](LocalPrinterItem *localPrinter, LocalPrinterProperties_t properties) {
            processLocalPrinter(localPrinter, properties);
        };

    std::function<void(SharedPrinterItem *, SharedPrinterProperties_t)> sharedPrinterHandler =
        [&](SharedPrinterItem *sharedPrinter, SharedPrinterProperties_t properties) {
            processSharedPrinter(sharedPrinter, properties);
        };

    std::function<void(TcpPrinterItem *, PortPrinterProperties_t)> tcpPrinterHandler =
        [&](TcpPrinterItem *tcpPrinter, PortPrinterProperties_t properties) {
            processTcpPrinter(tcpPrinter, properties);
        };

    processLocalPrinters<Printers::LocalPrinterSequence, LocalPrinterItem, LocalPrinterProperties_t>(
        printersSource->LocalPrinter(), model, localPrinterHandler);

    processLocalPrinters<Printers::SharedPrinterSequence, SharedPrinterItem, SharedPrinterProperties_t>(
        printersSource->SharedPrinter(), model, sharedPrinterHandler);

    processLocalPrinters<Printers::PortPrinterSequence, TcpPrinterItem, PortPrinterProperties_t>(printersSource
                                                                                                     ->PortPrinter(),
                                                                                                 model,
                                                                                                 tcpPrinterHandler);

    return model;
}

std::unique_ptr<Printers> PrintersModelBuilder::modelToSchema(std::unique_ptr<PreferencesModel> &model)
{
    Q_UNUSED(model);

    return nullptr;
}

} // namespace preferences
