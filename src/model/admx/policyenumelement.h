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

#ifndef GPUI_POLICYENUMELEMENT_H
#define GPUI_POLICYENUMELEMENT_H

#include "../model.h"
#include "policyelement.h"

#include <map>
#include <memory>
#include <string>

namespace model
{
    namespace admx
    {
        struct EnumValue
        {
            EnumValue()
            {}
        };

        struct DecimalValue : EnumValue
        {
            unsigned int value;

            DecimalValue(unsigned int value)
                : value(value)
            {}
        };

        struct LongDecimalValue : EnumValue
        {
            unsigned long long value;

            LongDecimalValue(unsigned long long value)
                : value(value)
            {}
        };

        struct StringValue : EnumValue
        {
            std::string value;

            StringValue(const std::string& value)
                : value(value)
            {}
        };

        /*!
         * \brief An enumeration element in a policy.
         */
        class GPUI_MODEL_EXPORT PolicyEnumElement : public PolicyElement
        {
        public:
            /*!
             * \brief True if the value is required.
             */
            bool required = false;

            /*!
             * \brief items List of enum items.
             */
            std::map<std::string, std::unique_ptr<EnumValue> > items;
        };
    }
}

#endif // GPUI_POLICYENUMELEMENT_H
