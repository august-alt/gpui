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

#ifndef GPUI_PRINTERS_MODEL_BUILDER_H
#define GPUI_PRINTERS_MODEL_BUILDER_H

#include <memory>

#include "common/basemodelbuilder.h"
#include "common/preferencesmodel.h"
#include "schemas/printersschema.h"

namespace preferences
{
class LocalPrinterItem;
class SharedPrinterItem;
class TcpPrinterItem;

class PrintersModelBuilder : public BaseModelBuilder
{
public:
    PrintersModelBuilder();

    std::unique_ptr<PreferencesModel> schemaToModel(std::unique_ptr<Printers>& printers);

    std::unique_ptr<Printers> modelToSchema(std::unique_ptr<PreferencesModel>& model);

private:
    template <typename PrinterSourceType, typename ItemType, typename PropertiesType>
    void processLocalPrinters(PrinterSourceType &printersSource, std::unique_ptr<PreferencesModel>& model,
                              std::function<void (ItemType*, PropertiesType)>& callback);

    void processLocalPrinter(LocalPrinterItem* localPrinter, LocalPrinterProperties_t properties);
    void processSharedPrinter(SharedPrinterItem* sharedPrinter, SharedPrinterProperties_t properties);
    void processTcpPrinter(TcpPrinterItem* tcpPrinter, PortPrinterProperties_t properties);
};
}

#endif // GPUI_PRINTERS_MODEL_BUILDER_H

