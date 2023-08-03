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

#ifndef SECURITY_COMBOBOX_H
#define SECURITY_COMBOBOX_H

#include "security.h"

#include "dataelementcontent.h"

#include <string>
#include <vector>

namespace security
{

/*!
   \class ComboBox
 * \brief Represents a combobox display element with default/suggested entries.
 *
 * \ingroup security
 * \ingroup presentation
 */
class ComboBox: public DataElementContent
{
public:
    /*!
     * \brief label Text associated with the input box to provide prompt text.
     */
    std::string label {};

    /*!
     * \brief defaultValue Specifies a default value. This can be used for either string or numeric data.
     */
    std::string defaultValue {};

    /*!
     * \brief suggestions A suggested value to be placed in the drop-down list.
     * Multiple suggestion elements result in multiple suggestions.
     */
    std::string suggestion {};

    /*!
     * \brief noSort If elements should be sorted.
     */
    bool noSort { false };
};

} // of namespace security

#endif // SECURITY_COMBOBOX_H
