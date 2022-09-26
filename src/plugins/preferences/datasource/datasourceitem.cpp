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

#include "datasourceitem.h"

#include "attributes/attributesitem.h"

namespace preferences
{

DataSourceItem::DataSourceItem()
    : ModelView::CompoundItem("DataSourceItem")
{
    addProperty(ACTION, "");
    addProperty(USERDSN, "");
    addProperty(DSN, "");
    addProperty(DRIVER, "");
    addProperty(DESCRIPTION, "");
    addProperty(USERNAME, "");
    addProperty(CPASSWORD, "");
    addProperty(USERSOURCE, false);
    addProperty(SYSTEMSOURCE, !property<bool>(USERSOURCE));

    addProperty<AttributesItem>(ATTRIBUTES)->setVisible(false);
}

DataSourceItem::DataSourceItem(const DataSourceItem &other)
    : ModelView::CompoundItem("DataSourceItem")
{
    addProperty(ACTION, other.property<std::string>(ACTION));
    addProperty(USERDSN, other.property<std::string>(USERDSN));
    addProperty(DSN, other.property<std::string>(DSN));
    addProperty(DRIVER, other.property<std::string>(DRIVER));
    addProperty(USERDSN, other.property<std::string>(USERDSN));
    addProperty(DESCRIPTION, other.property<std::string>(DESCRIPTION));
    addProperty(USERNAME, other.property<std::string>(USERNAME));
    addProperty(CPASSWORD, other.property<std::string>(CPASSWORD));
    addProperty(USERSOURCE, other.property<bool>(USERSOURCE));
    addProperty(SYSTEMSOURCE, !other.property<bool>(USERSOURCE));
    addProperty(ATTRIBUTES, other.property<AttributesItem>(ATTRIBUTES));
}

}
