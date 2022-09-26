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

#include "openwithitem.h"

namespace preferences
{

OpenWithItem::OpenWithItem()
    : ModelView::CompoundItem("OpenWithItem")
{
    addProperty(ACTION, "");
    addProperty(FILE_EXTENSION, "");
    addProperty(APPLICATION_PATH, "");
    addProperty(DEFAULT, false);
}

OpenWithItem::OpenWithItem(const OpenWithItem &other)
    : ModelView::CompoundItem("OpenWithItem")
{
    addProperty(ACTION, other.property<QString>(ACTION));
    addProperty(FILE_EXTENSION, other.property<std::string>(FILE_EXTENSION));
    addProperty(APPLICATION_PATH, other.property<std::string>(APPLICATION_PATH));
    addProperty(DEFAULT, other.property<bool>(DEFAULT));
}

}
