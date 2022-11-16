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

#ifndef GPUI_SHORTCUTS_ITEM_H
#define GPUI_SHORTCUTS_ITEM_H

#include <mvvm/model/compounditem.h>

namespace preferences
{
//! Drives item representation for editor.

class ShortcutsItem : public ModelView::CompoundItem
{
public:
    static inline const std::string ACTION        = "action";
    static inline const std::string PIDL          = "pidl";
    static inline const std::string SHORTCUT_PATH = "shortcutPath";
    static inline const std::string TARGET_TYPE   = "targetType";
    static inline const std::string TARGET_PATH   = "targetPath";
    static inline const std::string LOCATION      = "location";
    static inline const std::string ARGUMENTS     = "arguments";
    static inline const std::string START_IN      = "startIn";
    static inline const std::string SHORTCUT_KEY  = "shortcutKey";
    static inline const std::string WINDOW        = "window";
    static inline const std::string COMMENT       = "comment";
    static inline const std::string ICON_PATH     = "iconPath";
    static inline const std::string ICON_INDEX    = "iconIndex";

    ShortcutsItem();
    ShortcutsItem(const ShortcutsItem &other);
};

} // namespace preferences

Q_DECLARE_METATYPE(::preferences::ShortcutsItem)

#endif //GPUI_SHORTCUTS_ITEM_H
