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

#ifndef GPUI_CATEGORY_H
#define GPUI_CATEGORY_H

#include "../model.h"

#include <string>
#include <vector>

namespace model {
    namespace admx {
        /*!
         * \brief The category element specifies the name of a unique category to be displayed in the editor.
         */
        class GPUI_MODEL_EXPORT Category
        {
        public:
            /*!
             * \brief name Specifies a logical name to use for a specific supported application and revision.
             */
            std::string name;

            /*!
             * \brief displayName The reference to the category text string located in the string table of the .adml file.
             */
            std::string displayName;

            /*!
             * \brief explainText Explain or Help text associated with a specific category.
             */
            std::string explainText;

            /*!
             * \brief keywords Contains index words that could be used to search for elements.
             */
            std::vector<std::string> keywords;

            /*!
             * \brief parentCategory Reference to parent of the current category.
             */
            std::string parentCategory;

            /*!
             * \brief seeAlso Reference to another element that may be related.
             */
            std::vector<std::string> seeAlso;

            virtual ~Category() = default;
        };
    }
}

#endif // GPUI_CATEGORY_H
