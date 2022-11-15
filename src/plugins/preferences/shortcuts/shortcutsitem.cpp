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

#include "shortcutsitem.h"

#include <QKeySequence>

namespace preferences
{
ShortcutsItem::ShortcutsItem()
    : ModelView::CompoundItem("ShortcutsItem")
{
    addProperty(ACTION, 0);
    addProperty(PIDL, "");
    addProperty(SHORTCUT_PATH, "");
    addProperty(TARGET_TYPE, "");
    addProperty(TARGET_PATH, "");
    addProperty(ARGUMENTS, "");
    addProperty(START_IN, "");
    addProperty(SHORTCUT_KEY, "");
    addProperty(WINDOW, 0);
    addProperty(COMMENT, "");
    addProperty(ICON_PATH, "");
    addProperty(ICON_INDEX, "");
    addProperty(LOCATION, 0);
}

ShortcutsItem::ShortcutsItem(const ShortcutsItem &other)
    : ModelView::CompoundItem("ShortcutsItem")
{
    addProperty(ACTION, other.property<int>(ACTION));
    addProperty(PIDL, other.property<std::string>(PIDL));
    addProperty(SHORTCUT_PATH, other.property<std::string>(SHORTCUT_PATH));
    addProperty(TARGET_TYPE, other.property<std::string>(TARGET_TYPE));
    addProperty(TARGET_PATH, other.property<std::string>(TARGET_PATH));
    addProperty(ARGUMENTS, other.property<std::string>(ARGUMENTS));
    addProperty(START_IN, other.property<std::string>(START_IN));
    addProperty(SHORTCUT_KEY, other.property<std::string>(SHORTCUT_KEY));
    addProperty(WINDOW, other.property<int>(WINDOW));
    addProperty(COMMENT, other.property<std::string>(COMMENT));
    addProperty(ICON_PATH, other.property<std::string>(ICON_PATH));
    addProperty(ICON_INDEX, other.property<std::string>(ICON_INDEX));
    addProperty(LOCATION, other.property<int>(LOCATION));
}

} // namespace preferences
