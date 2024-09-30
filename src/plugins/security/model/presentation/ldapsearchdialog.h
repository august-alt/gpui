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

#ifndef SECURITY_LDAP_SEARCH_DIALOG_H
#define SECURITY_LDAP_SEARCH_DIALOG_H

#include "dataelementcontent.h"

#include <string>

namespace security
{

/*!
 * \class LdapSearchDialog
 * \brief Represents a LDAP search dialog display element.\nCan be associated with a TextElement.
 *
 * \ingroup security
 * \ingroup presentation
 */
class LdapSearchDialog: public DataElementContent
{
public:
    /*!
     * \brief dn Base DN for elements to search.
     */
    std::string dn{};

    /*!
     * \brief filter LDAP search filter.
     */
    std::string filter{};

    /*!
     * \brief addLabel Label for add button.
     */
    std::string addLabel{};

    /*!
     * \brief removeLabel Label for remove button.
     */
    std::string removeLabel{};

    /*!
     * \brief title Label for group box.
     */
    std::string title{};
};

} // of namespace security

#endif // SECURITY_LDAP_SEARCH_DIALOG_H
