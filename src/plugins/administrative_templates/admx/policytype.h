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

#ifndef GPUI_POLICYTYPE_H
#define GPUI_POLICYTYPE_H

namespace model
{
namespace admx
{
/*!
 * \brief The PolicyType enum can be defined as "User," "Machine," or "Both."
 * Identifies whether the policy will be located under a single (computer or user) or both nodes.
 */
enum class PolicyType
{
    User    = 0,
    Machine = 1,
    Both    = 2
};
} // namespace admx
} // namespace model

#endif // GPUI_POLICYTYPE_H
