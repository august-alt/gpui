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

#ifndef SECURITY_SECURITY_ITEM_H
#define SECURITY_SECURITY_ITEM_H

#include "security.h"

#include "securityvalue.h"

#include <string>

namespace security {

/*!
 * \class SecurityItem
 * \brief The SecurityItem class represents element of security policy.
 *
 * \ingroup security
 *
 * \see SecurityValue
 */
class SecurityItem
{
public:
    /*!
     * \brief sectionName Name of the section to place item into.
     */
    std::string sectionName {};

    /*!
     * \brief propertyName Name of the property associated with this security policy element.
     */
    std::string propertyName {};

    /*!
     * \brief value Value of current element.
     */
    SecurityValue value {};
};

} // of namespace security

#endif // SECURITY_SECURITY_ITEM_H
