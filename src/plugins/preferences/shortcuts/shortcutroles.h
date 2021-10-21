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

#ifndef GPUI_SHORTCUT_ROLES_H
#define GPUI_SHORTCUT_ROLES_H

#include "../common/commonroles.h"

namespace gpui
{
    enum ShortcutRoles
    {
        ACTION                  = CommonRoles::ALL_COMMON_ROLES + 1,
        ARGUMENTS               = CommonRoles::ALL_COMMON_ROLES + 2,
        COMMENT                 = CommonRoles::ALL_COMMON_ROLES + 3,
        ICON_INDEX              = CommonRoles::ALL_COMMON_ROLES + 4,
        ICON_PATH               = CommonRoles::ALL_COMMON_ROLES + 5,
        PIDL                    = CommonRoles::ALL_COMMON_ROLES + 6,
        SHORTCUT_KEY            = CommonRoles::ALL_COMMON_ROLES + 7,
        SHORTCUT_PATH           = CommonRoles::ALL_COMMON_ROLES + 8,
        START_IN                = CommonRoles::ALL_COMMON_ROLES + 9,
        TARGET_PATH             = CommonRoles::ALL_COMMON_ROLES + 10,
        TARGET_TYPE             = CommonRoles::ALL_COMMON_ROLES + 11,
        WINDOW                  = CommonRoles::ALL_COMMON_ROLES + 12,

        ALL_SHORTCUT_ROLES
    };
}

#endif // GPUI_SHORTCUT_ROLES_H
