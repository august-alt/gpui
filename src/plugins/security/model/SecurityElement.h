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
#ifndef SECURITY_SECURITYELEMENT_H
#define SECURITY_SECURITYELEMENT_H

/************************************************************
 SecurityElement class header
 ************************************************************/

#include "security/Pkg_security.h"

#include <string>

namespace security {

/************************************************************/
/**
 * 
 */
class SecurityElement {
public:

	/**
	 * 
	 */
	std::string refId;

	/**
	 * 
	 */
	std::string clientExtension;

	/**
	 * 
	 */
	std::string propertyName;

};
/************************************************************/
/* External declarations (package visibility)               */
/************************************************************/

/* Inline functions                                         */

} // of namespace security

/************************************************************
 End of SecurityElement class header
 ************************************************************/

#endif
