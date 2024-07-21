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
BooleanValue::BooleanValue()
    : m_type(BOOLEAN_VALUE_TYPE_DELETED), m_has_key(false), m_has_value_name(false)
{
}

/*!
 * \brief Construct BooleanValue with decimal value.
 */
BooleanValue::BooleanValue(unsigned int decimalValue)
    : m_type(BOOLEAN_VALUE_TYPE_DECIMAL),
      m_has_key(false),
      m_has_value_name(false),
      m_decimal(decimalValue)
{
}

/*!
 * \brief Construct BooleanValue with long decimal value.
 */
BooleanValue::BooleanValue(unsigned long long longDecimalValue)
    : m_type(BOOLEAN_VALUE_TYPE_LONGDECIMAL),
      m_has_key(false),
      m_has_value_name(false),
      m_long_decimal(longDecimalValue)
{
}

/*!
 * \brief Construct BooleanValue with string value
 */
BooleanValue::BooleanValue(const std::string &string)
    : m_type(BOOLEAN_VALUE_TYPE_STRING), m_has_key(false), m_has_value_name(false), m_string(string)
{
}

/*!
 * \brief Copy-constructor
 */
BooleanValue::BooleanValue(const BooleanValue &value)
    : m_type(value.m_type), m_has_key(false), m_has_value_name(false)
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
BooleanValue::BooleanValue(BooleanValue &&value)
    : m_type(value.m_type), m_has_key(false), m_has_value_name(false)
{
    switch (this->m_type) {
    case BOOLEAN_VALUE_TYPE_DECIMAL:
        this->m_decimal = value.m_decimal;
        break;
    case BOOLEAN_VALUE_TYPE_LONGDECIMAL:
        this->m_long_decimal = value.m_long_decimal;
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
 * \brief Set BooleanValue long decimal value.
 */
void BooleanValue::setValue(unsigned long long longDecimal)
{
    if (this->m_type == BOOLEAN_VALUE_TYPE_STRING) {
        this->m_string.~basic_string();
    }
    this->m_type = BOOLEAN_VALUE_TYPE_LONGDECIMAL;
    this->m_long_decimal = longDecimal;
}

/*!
 * \brief Set value name.
 */
void BooleanValue::setValueName(const std::string &valueName)
{
    this->m_value_name = valueName;
}
/*!
 * \brief Set key.
 */
void BooleanValue::setKey(const std::string &key)
{
    this->m_key = key;
}

/*!
 * \brief Return value name.
 */
const std::string &BooleanValue::valueName() const
{
    return this->m_value_name;
}
/*!
 * \brief Return key.
 */
const std::string &BooleanValue::key() const
{
    return this->m_key;
}

/*!
 * \brief Return string value.
 */
BooleanValue::Type BooleanValue::type() const
{
    return this->m_type;
}

/*!
 * \brief Return string value.
 * \throw std::runtime_error if BooleanValue type is't string
 */
const std::string &BooleanValue::string() const
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
unsigned int BooleanValue::decimal() const
{
    if (this->m_type != BOOLEAN_VALUE_TYPE_DECIMAL) {
        throw std::runtime_error("wrong type for BooleanValue");
    }

    return this->m_decimal;
}
/*!
 * \brief Return decimal value.
 * \throw std::runtime_error if BooleanValue type is't decimal
 */
unsigned long long BooleanValue::longDecimal() const
{
    if (this->m_type != BOOLEAN_VALUE_TYPE_LONGDECIMAL) {
        throw std::runtime_error("wrong type for BooleanValue");
    }

    return this->m_long_decimal;
}

/*!
 * \brief Return true if key is present.
 */
bool BooleanValue::keyPresent() const
{
    return this->m_has_key;
}
/*!
 * \brief Return true if value name is present.
 */
bool BooleanValue::valuePresent() const
{
    return this->m_has_value_name;
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
