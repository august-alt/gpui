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

#ifndef GPUI_POLICYLISTELEMENT_H
#define GPUI_POLICYLISTELEMENT_H

#include "../model.h"
#include "policyelement.h"

#include <string>

namespace model
{
    namespace admx
    {
        /*!
         * \brief The PolicyListElement class sets a list of different registry subkeys when a policy setting is enabled.
         */
        class PolicyListElement : public PolicyElement
        {
        public:
            /*!
             * \brief A prefix for value.
             */
            std::string valuePrefix;

            /*!
             * \brief additive
             */
            bool additive = false;

            /*!
             * \brief expandable
             */
            bool expandable = false;

            /*!
             * \brief explicitValue
             */
            bool explicitValue = false;
        };
    }
}

#endif // GPUI_POLICYLISTELEMENT_H
