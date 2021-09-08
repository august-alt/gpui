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

#ifndef GPUI_COMMON_ROLES_H
#define GPUI_COMMON_ROLES_H

namespace gpui
{
    enum CommonRoles
    {
        CLSID           = 0,
        DISABLED        = 1,
        NAME            = 2,
        STATUS          = 3,
        IMAGE           = 4,
        CHANGED         = 5,
        UID             = 6,
        DESC            = 7,
        BYPASS_ERRORS   = 8,
        USER_CONTEXT    = 9,
        REMOVE_POLICY   = 10,

        ALL_COMMON_ROLES = REMOVE_POLICY + 1
    };
}

#endif // GPUI_COMMON_ROLES_H
