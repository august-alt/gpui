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

#ifndef GPUI_BOOLEAN_VALUE_H
#define GPUI_BOOLEAN_VALUE_H

#include <string>
#include <stdexcept>

namespace model {
namespace admx {
/*!
 * \brief Describes a value that is equivalent to some logical state (true, false)
 */
class BooleanValue
{
public:
    /*!
     * \brief Enumeration of all possible value types
     */
    enum Type {
        BOOLEAN_VALUE_TYPE_DELETED,
        BOOLEAN_VALUE_TYPE_STRING,
        BOOLEAN_VALUE_TYPE_DECIMAL,
        BOOLEAN_VALUE_TYPE_LONGDECIMAL,
    };

    /*!
     * \brief Type of boolean value
     */
    Type type{ BOOLEAN_VALUE_TYPE_DELETED };
    /*!
     * \brief True if the key is present, i.e. false
     */
    bool has_key{ false };

    /*!
     * \brief Contain value name
     */
    std::string value_name{};
    /*!
     * \brief Contain the key if the key is present, i.e. UB
     */
    std::string key{};

    /*!
     * \brief Contains a decimal number if the type is BOOLEAN_VALUE_TYPE_DECIMAL, i.e. UB
     */
    unsigned int decimal{};
    /*!
     * \brief Contains a long decimal number if the type is BOOLEAN_VALUE_TYPE_LONGDECIMAL, i.e. UB
     */
    unsigned long long long_decimal{};
    /*!
     * \brief Contains a string if the type is BOOLEAN_VALUE_TYPE_STRING, i.e. UB
     */
    std::string string{};
};
} // namespace admx
} // namespace model

#endif // GPUI_BOOLEAN_VALUE_H
