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

#include "filesitem.h"

namespace preferences
{
FilesItem::FilesItem()
    : ModelView::CompoundItem("FilesItem")
{
    addProperty(ACTION, 0);
    addProperty(FROM_PATH, "");
    addProperty(TARGET_PATH, "");
    addProperty(SUPPRESS, false);
    addProperty(READONLY, false);
    addProperty(ARCHIVE, false);
    addProperty(HIDDEN, false);
    addProperty(EXECUTABLE, false);
}

FilesItem::FilesItem(const FilesItem &other)
    : ModelView::CompoundItem("FilesItem")
{
    addProperty(ACTION, other.property<int>(ACTION));
    addProperty(FROM_PATH, other.property<QString>(FROM_PATH));
    addProperty(TARGET_PATH, other.property<QString>(TARGET_PATH));
    addProperty(SUPPRESS, other.property<bool>(SUPPRESS));
    addProperty(READONLY, other.property<bool>(READONLY));
    addProperty(ARCHIVE, other.property<bool>(ARCHIVE));
    addProperty(HIDDEN, other.property<bool>(HIDDEN));
    addProperty(EXECUTABLE, other.property<bool>(EXECUTABLE));
}

} // namespace preferences
