/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd.
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
         * \brief The PolicyTextElement class
         */
        class PolicyTextElement : public PolicyElement
        {
        public:
            /*!
             * \brief A valid registry value name.
             */
            std::string valueName;

            /*!
             * \brief True if the value is required.
             */
            bool required = false;

            /*!
             * \brief The maximum length of the string.
             */
            unsigned int maxLength = 1023;

            /*!
             * \brief True if the string can be expanded.
             */
            bool expandable = false;

            /*!
             * \brief soft
             */
            bool soft = false;
        };
    }
}

#endif // GPUI_POLICYTEXTELEMENT_H
