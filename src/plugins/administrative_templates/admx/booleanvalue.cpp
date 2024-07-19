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

#include "booleanvalue.h"
#include <new>

namespace model {
namespace admx {
/*!
 * \brief Describes a value that is equivalent to some logical state (true, false)
 */

/*!
 * \brief Construct BooleanValue with deleted value.
 */
BooleanValue::BooleanValue() : m_type(BOOLEAN_VALUE_TYPE_DELETED) { }

/*!
 * \brief Construct BooleanValue with decimal value.
 */
BooleanValue::BooleanValue(unsigned int decimalValue)
    : m_type(BOOLEAN_VALUE_TYPE_DECIMAL), m_decimal(decimalValue)
{
}

/*!
 * \brief Construct BooleanValue with string value
 */
BooleanValue::BooleanValue(const std::string &string)
    : m_type(BOOLEAN_VALUE_TYPE_STRING), m_string(string)
{
}

/*!
 * \brief Copy-constructor
 */
BooleanValue::BooleanValue(const BooleanValue &value) : m_type(value.m_type)
{
    switch (this->m_type) {
    case BOOLEAN_VALUE_TYPE_DECIMAL:
        this->m_decimal = value.m_decimal;
        break;
    case BOOLEAN_VALUE_TYPE_STRING:
        new (&this->m_string) std::string(value.m_string);
        break;
    default:
        break;
    }
}
/*!
 * \brief Move-constructor
 */
BooleanValue::BooleanValue(BooleanValue &&value) : m_type(value.m_type)
{
    switch (this->m_type) {
    case BOOLEAN_VALUE_TYPE_DECIMAL:
        this->m_decimal = value.m_decimal;
        break;
    case BOOLEAN_VALUE_TYPE_STRING:
        new (&this->m_string) std::string(std::move(value.m_string));
        break;
    default:
        break;
    }
    value.clear();
}

/*!
 * \brief Set Boolean value to deleted value
 */
void BooleanValue::clear()
{
    if (this->m_type == BOOLEAN_VALUE_TYPE_STRING) {
        this->m_string.~basic_string();
    }
    this->m_type = BOOLEAN_VALUE_TYPE_DELETED;
}

/*!
 * \brief Set BooleanValue deleted value.
 */
void BooleanValue::setValue()
{
    // BooleanValue::clear() do the same.
    this->clear();
}

/*!
 * \brief Set BooleanValue decimal value.
 */
void BooleanValue::setValue(unsigned int decimal)
{
    if (this->m_type == BOOLEAN_VALUE_TYPE_STRING) {
        this->m_string.~basic_string();
    }
    this->m_type = BOOLEAN_VALUE_TYPE_DECIMAL;
    this->m_decimal = decimal;
}

/*!
 * \brief Set BooleanValue string value.
 */
void BooleanValue::setValue(const std::string &string)
{
    if (this->m_type != BOOLEAN_VALUE_TYPE_STRING) {
        new (&this->m_string) std::string;
    }
    this->m_type = BOOLEAN_VALUE_TYPE_STRING;
    this->m_string = string;
}

/*!
 * \brief Return string value.
 */
BooleanValue::Type BooleanValue::type()
{
    return this->m_type;
}

/*!
 * \brief Return string value.
 * \throw std::runtime_error if BooleanValue type is't string
 */
const std::string &BooleanValue::string()
{
    if (this->m_type != BOOLEAN_VALUE_TYPE_STRING) {
        throw std::runtime_error("wrong type for BooleanValue");
    }

    return this->m_string;
}

/*!
 * \brief Return decimal value.
 * \throw std::runtime_error if BooleanValue type is't decimal
 */
double BooleanValue::decimal()
{
    if (this->m_type != BOOLEAN_VALUE_TYPE_DECIMAL) {
        throw std::runtime_error("wrong type for BooleanValue");
    }

    return this->m_decimal;
}

/*!
 * \brief Default destructor.
 */
BooleanValue::~BooleanValue()
{
    if (this->m_type == BOOLEAN_VALUE_TYPE_STRING) {
        this->m_string.~basic_string();
    }
    this->m_type = BOOLEAN_VALUE_TYPE_DELETED;
}
} // namespace admx
} // namespace model
