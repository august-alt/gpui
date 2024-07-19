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
     * \brief Construct BooleanValue with deleted value.
     */
    BooleanValue();
    /*!
     * \brief Construct BooleanValue with string value
     */
    BooleanValue(const std::string &string);
    /*!
     * \brief Construct BooleanValue with decimal value.
     */
    BooleanValue(unsigned int decimalValue);
    /*!
     * \brief Construct BooleanValue with long decimal value.
     */
    BooleanValue(unsigned long long longDecimalValue);

    /*!
     * \brief Copy-constructor
     */
    BooleanValue(const BooleanValue &value);
    /*!
     * \brief Move-constructor
     */
    BooleanValue(BooleanValue &&value);

    /*!
     * \brief Set Boolean value to deleted value
     */
    void clear();

    /*!
     * \brief Set BooleanValue deleted value.
     */
    void setValue();
    /*!
     * \brief Set BooleanValue string value.
     */
    void setValue(const std::string &string);
    /*!
     * \brief Set BooleanValue decimal value.
     */
    void setValue(unsigned int decimal);
    /*!
     * \brief Set BooleanValue decimal value.
     */
    void setValue(unsigned long long decimal);

    /*!
     * \brief Return string value.
     */
    Type type();

    /*!
     * \brief Return string value.
     * \throw std::runtime_error if BooleanValue type is't string
     */
    const std::string &string();
    /*!
     * \brief Return decimal value.
     * \throw std::runtime_error if BooleanValue type is't decimal
     */
    unsigned int decimal();
    /*!
     * \brief Return long decimal value.
     * \throw std::runtime_error if BooleanValue type is't decimal
     */
    unsigned long long longDecimal();

    /*!
     * \brief Default destructor.
     */
    ~BooleanValue();

private:
    Type m_type{};

    union {
        unsigned int m_decimal;
        unsigned long long m_long_decimal;
        std::string m_string;
    };
};
} // namespace admx
} // namespace model

#endif // GPUI_BOOLEAN_VALUE_H
