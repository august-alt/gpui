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

#ifndef GPUI_RESOURCES_H
#define GPUI_RESOURCES_H

#include "../model.h"
#include <string>

namespace model {
    namespace admx {
        /*!
         * \brief The Resources class The resourceselement, in the .admx file, specifies the minimum revision level
         * of the matching .adml file and optionally the fallback language to use.
         */
        class GPUI_MODEL_EXPORT Resources
        {
        public:
            /*!
             * \brief minRequiredRevision A minimum revision level of the matching .adml file.
             */
            std::string minRequiredRevision;

            /*!
             * \brief fallbackCulture Default language to use when the .adml file does not exist on the machine or ADMX
             * central store for the required language.
             */
            std::string fallbackCulture;
        };
    }
}

#endif // GPUI_RESOURCES_H
