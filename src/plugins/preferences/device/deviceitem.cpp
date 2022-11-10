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

#include "deviceitem.h"

#include <QUuid>

namespace preferences
{

DeviceItem::DeviceItem()
    : ModelView::CompoundItem("DeviceItem")
{
    addProperty(DEVICE_ACTION, "");
    addProperty(DEVICE_CLASS, "");
    addProperty(DEVICE_TYPE, "");
    addProperty(DEVICE_CLASS_GUID, QUuid());
    addProperty(DEVICE_TYPE_ID, "");
}

DeviceItem::DeviceItem(const DeviceItem &other)
    : ModelView::CompoundItem("DeviceItem")
{
    addProperty(DEVICE_ACTION, other.property<std::string>(DEVICE_ACTION));
    addProperty(DEVICE_CLASS, other.property<std::string>(DEVICE_CLASS));
    addProperty(DEVICE_TYPE, other.property<std::string>(DEVICE_TYPE));
    addProperty(DEVICE_CLASS_GUID, other.property<QUuid>(DEVICE_CLASS_GUID));
    addProperty(DEVICE_TYPE_ID, other.property<std::string>(DEVICE_TYPE_ID));
}

}
