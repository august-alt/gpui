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
        BYPASS_ERRORS   = 0,
        CHANGED         = 1,
        CLSID           = 2,
        DESC            = 3,
        DISABLED        = 4,
        IMAGE           = 5,
        NAME            = 6,
        REMOVE_POLICY   = 7,
        STATUS          = 8,
        UID             = 9,
        USER_CONTEXT    = 10,

        ALL_COMMON_ROLES
    };
}

#endif // GPUI_COMMON_ROLES_H
