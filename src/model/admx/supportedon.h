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

#ifndef GPUI_SUPPORTEDON_H
#define GPUI_SUPPORTEDON_H

#include "../model.h"

#include "supportedproduct.h"
#include "supporteddefinition.h"

#include <memory>

namespace model
{
    namespace admx
    {
        /*!
         * \brief The SupportedOn class The supportedOn element provides a mapping of products to definitions.
         */
        class GPUI_MODEL_EXPORT SupportedOn
        {
        public:
            /*!
             * \brief product The products element is a table of product elements for the supportedOn text.
             */
            std::unique_ptr<SupportedProduct> products;

            /*!
             * \brief definition The definitions element is a table of definition elements for the supportedOn text.
             */
            std::unique_ptr<SupportedDefinition> definitions;
        };
    }
}

#endif // GPUI_SUPPORTEDON_H
