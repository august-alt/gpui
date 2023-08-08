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

#ifndef SECURITY_DROPDOWNLIST_H
#define SECURITY_DROPDOWNLIST_H

#include "dataelementcontent.h"

#include <string>
#include <vector>

namespace security
{

/*!
 * \class DropdownList
 * \brief Represents a dropdown list display element.
 */
class DropdownList : public DataElementContent
{
public:
    /*!
     * \brief noSort
     */
    bool noSort { false };

    /*!
     * \brief defaultItem The numerical value identifying the default choice of the list of items.
     * The item list is numbered starting from 0.
     */
    unsigned int defaultItem { 0 };

    /*!
     * \brief label Text associated with the drop-down list.
     */
    std::string label{};

    /*!
     * \brief values Values of the drop-down list.
     */
    std::vector<std::string> values{};

    /*!
     * \brief refId A mapping to the element.
     */
    std::string refId{};
};

} // of namespace security

#endif // SECURITY_DROPDOWNLIST_H
