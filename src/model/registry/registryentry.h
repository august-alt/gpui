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

#ifndef GPUI_REGISTRY_ENTRY_H
#define GPUI_REGISTRY_ENTRY_H

#include "../model.h"

#include "abstractregistryentry.h"

namespace model {
    namespace registry {
        /*!
         * \brief The RegistryEntry class holds data for registry entry.
         */
        template <typename TData>
        class GPUI_MODEL_EXPORT RegistryEntry : public AbstractRegistryEntry
        {
        public:
            /*!
             * \brief data Indicates the data associated with the value.
             */
            TData data;
        };
    }
}

#endif // GPUI_REGISTRY_ENTRY_H
