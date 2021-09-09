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

#ifndef GPUI_DRIVE_ROLES_H
#define GPUI_DRIVE_ROLES_H

#include "../common/commonroles.h"

namespace gpui
{
    enum DriveRoles
    {
        ACTION                  = CommonRoles::ALL_COMMON_ROLES + 1,
        PATH                    = CommonRoles::ALL_COMMON_ROLES + 2,
        PERSISTENT              = CommonRoles::ALL_COMMON_ROLES + 3,
        LABEL                   = CommonRoles::ALL_COMMON_ROLES + 4,
        LETTER                  = CommonRoles::ALL_COMMON_ROLES + 5,
        USERNAME                = CommonRoles::ALL_COMMON_ROLES + 6,
        ENCRYPTED_PASSWORD      = CommonRoles::ALL_COMMON_ROLES + 7,
        USE_LETTER              = CommonRoles::ALL_COMMON_ROLES + 8,
        THIS_DRIVE              = CommonRoles::ALL_COMMON_ROLES + 9,
        ALL_DRIVES              = CommonRoles::ALL_COMMON_ROLES + 10,

        ALL_DRIVE_ROLES
    };
}

#endif // GPUI_DRIVE_ROLES_H
