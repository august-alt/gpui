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

#include "../model.h"
#include "policyelement.h"

#include <string>

namespace model
{
    namespace admx
    {
        /*!
         * \brief A choice element in a policy with associated values for the true and false cases.
         */
        class GPUI_MODEL_EXPORT PolicyBoolElement : public PolicyElement
        {
        public:
        };
    }
}

#endif // GPUI_POLICYboolELEMENT_H
