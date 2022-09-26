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

#include "registryitem.h"

namespace preferences
{

RegistryItem::RegistryItem()
    : ModelView::CompoundItem("RegistryItem")
{
    addProperty(ACTION, "");
    addProperty(HIVE, "");
    addProperty(KEY, "");
    addProperty(NAME, "");
    addProperty(TYPE, "");
    addProperty(VALUE, "");
    addProperty(DEFAULT, false);
}

RegistryItem::RegistryItem(const RegistryItem &other)
    : ModelView::CompoundItem("RegistryItem")
{
    addProperty(ACTION, other.property<std::string>(ACTION));
    addProperty(HIVE, other.property<std::string>(HIVE));
    addProperty(KEY, other.property<std::string>(KEY));
    addProperty(NAME, other.property<std::string>(NAME));
    addProperty(TYPE, other.property<std::string>(TYPE));
    addProperty(VALUE, other.property<std::string>(VALUE));
    addProperty(DEFAULT, other.property<bool>(DEFAULT));
}

}
