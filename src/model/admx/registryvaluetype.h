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

#ifndef GPUI_REGISTRYVALUETYPE_H
#define GPUI_REGISTRYVALUETYPE_H

#include "../model.h"

namespace model
{
    namespace admx
    {
        /*!
         * \brief The RegistryValueType enum You must include a choice of either the delete element, decimal element,
         * or string element. Older version also contains long decimal element.
         */
        enum class GPUI_MODEL_EXPORT RegistryValueType
        {
            DECIMAL      = 0,
            LONG_DECIMAL = 1,
            STRING       = 2,
            DELETE       = 3
        };
    }
}

#endif  //_REGISTRYVALUETYPE_H
