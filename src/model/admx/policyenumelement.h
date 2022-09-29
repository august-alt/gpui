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

            virtual ~EnumValue() = default;
        };

        struct DecimalValue : EnumValue
        {
            unsigned int value = 0;

            DecimalValue(unsigned int newValue)
                : value(newValue)
            {}
        };

        struct LongDecimalValue : EnumValue
        {
            unsigned long long value = 0;

            LongDecimalValue(unsigned long long newValue)
                : value(newValue)
            {}
        };

        struct StringValue : EnumValue
        {
            std::string value {};

            StringValue(const std::string& newValue)
                : value(newValue)
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
            std::map<std::string, std::unique_ptr<EnumValue> > items {};

            registry::RegistryEntryType getRegistryEntryType() const override
            {
                if (items.size() > 0)
                {
                    const size_t hash_code = typeid(*items.begin()->second.get()).hash_code();
                    if (hash_code == typeid(DecimalValue).hash_code())
                    {
                        return registry::REG_DWORD;
                    }
                    else if (hash_code == typeid(LongDecimalValue).hash_code())
                    {
                        return registry::REG_QWORD;
                    }
                    else if (hash_code == typeid(StringValue).hash_code())
                    {
                        return registry::REG_SZ;
                    }
                }

                return registry::REG_SZ;
            }
        };
    }
}

#endif // GPUI_POLICYENUMELEMENT_H
