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

#ifndef GPUI_POLICYELEMENT_H
#define GPUI_POLICYELEMENT_H

#include "../registry/registryentrytype.h"

#include <string>

#include <QUuid>

namespace model
{
namespace admx
{
/*!
 * \brief The PolicyElement class represents the types of optional parameters defined in a policy setting.
 */
class PolicyElement
{
public:
    /*!
     * \brief Identifies the element.
     */
    std::string id{};

    /*!
     * \brief Requirement to enter a value for the element.
     */
    bool required = false;

    /*!
     * \brief The GUID of the extension.
     */
    QUuid clientExtension{};

    /*!
     * \brief A valid registry key path (without reference to local system or user hive).
     */
    std::string key{};

    /*!
     * \brief A valid registry value name.
     */
    std::string valueName{};

    virtual ~PolicyElement() = default;

    /*!
     * \brief getRegistryEntryType Returns type of registry entry associated with current policy element.
     * \return Type of registry entry associated with current policy element.
     */
    virtual registry::RegistryEntryType getRegistryEntryType() const { return registry::RegistryEntryType::REG_NONE; };
};
} // namespace admx
} // namespace model

#endif // GPUI_POLICYELEMENT_H
