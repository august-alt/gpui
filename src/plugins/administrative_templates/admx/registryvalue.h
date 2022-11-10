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

#ifndef GPUI_REGISTRYVALUE_H
#define GPUI_REGISTRYVALUE_H

#include "registryvaluetype.h"

namespace model
{
namespace admx
{
/*!
 * \brief The RegistryValue class The value element represents the actions to delete a registry subkey
 * or set the values of the registry subkey to a decimal or string value.
 */
class AbstractRegistryValue
{
public:
    /*!
     * \brief type You must include a choice of either the delete element, decimal element, or string element.
     */
    RegistryValueType type = RegistryValueType::DECIMAL;

    virtual ~AbstractRegistryValue() = default;
};

template<typename TValueType>
class RegistryValue : public AbstractRegistryValue
{
public:
    /*!
     * \brief value
     */
    TValueType value;

    RegistryValue(const TValueType &newValue)
        : value(newValue)
    {}
};
} // namespace admx
} // namespace model

#endif // GPUI_REGISTRYVALUE_H
