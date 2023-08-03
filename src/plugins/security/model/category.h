/***********************************************************************************************************************
 **
 ** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#ifndef SECURITY_CATEGORY_H
#define SECURITY_CATEGORY_H

#include "security.h"

#include <string>
#include <vector>

namespace security
{

/*!
 * \class Category
 * \brief A grouping of security definitions.
 *
 * \ingroup model
 * \ingroup presentation
 */
class Category
{
public:
    /*!
     * \brief name Specifies a logical name to use for a specific supported application and revision.
     */
    std::string name{};

    /*!
     * \brief displayName The reference to the category text string located in the string table of the .sdml file.
     */
    std::string displayName{};

    /*!
     * \brief keywords Contains index words that could be used to search for elements.
     */
    std::vector<std::string> keywords{};

    /*!
     * \brief seeAlso Reference to another element that may be related.
     */
    std::vector<std::string> seeAlso{};

    /*!
     * \brief explainText Explain or Help text associated with a specific category.
     */
    std::string explainText{};

    /*!
     * \brief parentCategory Reference to parent of the current category.
     */
    std::string parentCategory{};
};

} // of namespace security

#endif // SECURITY_CATEGORY_H
