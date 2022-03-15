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

#ifndef GPUI_SUPPORTEDPRODUCT_H
#define GPUI_SUPPORTEDPRODUCT_H

#include "../model.h"

#include "majorversion.h"

#include <vector>

namespace model
{
    namespace admx
    {
        /*!
         * \brief The SupportedProduct class Represents supported product.
         */
        class GPUI_MODEL_EXPORT SupportedProduct
        {
        public:
            /*!
             * \brief name Specifies a logical name to use for a specific supported application and revision.
             */
            std::string name {};

            /*!
             * \brief displayName The reference to the supported information string located in the
             * string table of the .adml file.
             */
            std::string displayName {};

            /*!
             * \brief majorVersion A product version.
             */
            std::vector<MajorVersion> majorVersion {};

            virtual ~SupportedProduct() = default;
        };
    }
}

#endif // GPUI_SUPPORTEDPRODUCT_H
