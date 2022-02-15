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

#ifndef GPUI_POLICYTEXTELEMENT_H
#define GPUI_POLICYTEXTELEMENT_H

#include "../model.h"
#include "policyelement.h"

#include <string>

namespace model
{
    namespace admx
    {
        /*!
         * \brief The PolicyTextElement class represents a text element in a policy.
         */
        class GPUI_MODEL_EXPORT PolicyTextElement : public PolicyElement
        {
        public:
            /*!
             * \brief Requirement to enter a value in the parameter box.
             */
            bool required = false;

            /*!
             * \brief The maximum length of the string.
             */
            unsigned int maxLength = 1023;

            /*!
             * \brief The registry subkey value will be created as an expandable string type (REG_EXPAND_SZ)
             * instead of a string type (REG_SZ).
             */
            bool expandable = false;

            /*!
             * \brief soft This attribute flags whether to overwrite an existing subkey.
             */
            bool soft = false;

            registry::RegistryEntryType getRegistryEntryType() const override
            {
                return expandable ? registry::REG_SZ : registry::REG_EXPAND_SZ;
            }
        };
    }
}

#endif // GPUI_POLICYTEXTELEMENT_H
