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

#ifndef GPUI_POLICYboolELEMENT_H
#define GPUI_POLICYboolELEMENT_H

#include "policyelement.h"
#include "booleanvalue.h"

#include <string>
#include <vector>

namespace model
{
    namespace admx
    {
        /*!
         * \brief A choice element in a policy with associated values for the true and false cases.
         */
        class PolicyBoolElement : public PolicyElement
        {
        public:
            typedef std::vector<BooleanValue> BooleanList;

            /*!
             * \brief The trueValue element sets the registry value for the true condition of a boolean
             * element.
             */
            BooleanList trueList{};
            /*!
             * \brief The falseValue element sets the registry value for the true condition of a boolean
             * element.
             */
            BooleanList falseList{};

            /*!
             * \brief Information about the existence of trueList
             */
            bool hasTrueList{};
            /*!
             * \brief Information about the existence of falseList
             */
            bool hasFalseList{};

            registry::RegistryEntryType getRegistryEntryType() const override
            {
                return registry::REG_DWORD;
            }
        };
    } // namespace admx
} // namespace model

#endif // GPUI_POLICYboolELEMENT_H
