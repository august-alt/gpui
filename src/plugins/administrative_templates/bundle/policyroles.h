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

#ifndef GPUI_POLICY_ROLES_H
#define GPUI_POLICY_ROLES_H

#include <QAbstractItemModel>

namespace model
{
namespace bundle
{
enum PolicyRoles
{
    ITEM_TYPE         = Qt::UserRole + 1,
    EXPLAIN_TEXT      = Qt::UserRole + 2,
    SUPPORTED_ON_TEXT = Qt::UserRole + 3,
    SUPPORTED_ON      = Qt::UserRole + 4,
    PRESENTATION      = Qt::UserRole + 5,
    POLICY            = Qt::UserRole + 6,
    POLICY_TYPE       = Qt::UserRole + 7,
    POLICY_WIDGET     = Qt::UserRole + 8,
    HAS_PARENT        = Qt::UserRole + 9,
    CURRENT_UUID      = Qt::UserRole + 12,
    PARENT_UUID       = Qt::UserRole + 13,
};
}
} // namespace model

#endif // GPUI_POLICY_ROLES_H
