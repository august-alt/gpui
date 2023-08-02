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
#ifndef SECURITY_LDAPSEARCHDIALOG_H
#define SECURITY_LDAPSEARCHDIALOG_H

/************************************************************
 LdapSearchDialog class header
 ************************************************************/

#include "security/Pkg_security.h"

#include "security/DataElementContent.h"

#include <string>

namespace security {

/************************************************************/
/**
 * Represents a LDAP search dialog display element.\nCan be associated with a TextElement.
 */
class LdapSearchDialog: public DataElementContent {
public:

	/**
	 * 
	 */
	std::string dn;

	/**
	 * 
	 */
	std::string filter;

	/**
	 * 
	 */
	std::string addLabel;

	/**
	 * 
	 */
	std::string removeLabel;

};
/************************************************************/
/* External declarations (package visibility)               */
/************************************************************/

/* Inline functions                                         */

} // of namespace security

/************************************************************
 End of LdapSearchDialog class header
 ************************************************************/

#endif
