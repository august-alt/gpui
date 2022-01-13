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

#ifndef GPUI_POLICYLONGDECIMALELEMENT_H
#define GPUI_POLICYLONGDECIMALELEMENT_H

#include "../model.h"
#include "policyelement.h"

#include <string>

namespace model
{
    namespace admx
    {
        /*!
         * \brief A QWORD (64-bit unsigned integer) number/decimal element in a policy.
         */
        class GPUI_MODEL_EXPORT PolicyLongDecimalElement : public PolicyElement
        {
        public:
            /*!
             * \brief True if the value is required.
             */
            bool required = false;

            /*!
             * \brief The minimum value of the element.
             */
            unsigned long minValue = 0;

            /*!
             * \brief The maximun value of the element.
             */
            unsigned long maxValue = 9999;

            /*!
             * \brief Set to true to store the value as text.
             */
            bool storeAsText = false;

            /*!
             * \brief soft This attribute flags whether to overwrite an existing subkey.
             */
            bool soft = false;

            registry::RegistryEntryType getRegistryEntryType() const override
            {
                return registry::REG_QWORD;
            }
        };
    }
}

#endif  //GPUI_POLICYLONGDECIMALELEMENT_H
