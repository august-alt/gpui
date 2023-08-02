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
#ifndef SECURITY_SECURITYDEFINITIONS_H
#define SECURITY_SECURITYDEFINITIONS_H

/************************************************************
 SecurityDefinitions class header
 ************************************************************/

#include "security/Pkg_security.h"

#include "security/SecurityDefinition.h"

namespace security {

/************************************************************/
/**
 * 
 */
class SecurityDefinitions {
public:

	/**
	 * 
	 */
	std::string revision;

	/**
	 * 
	 */
	std::string schemaVersion;

	/**
	 * 
	 */
	SecurityDefinition security[];

};
/************************************************************/
/* External declarations (package visibility)               */
/************************************************************/

/* Inline functions                                         */

} // of namespace security

/************************************************************
 End of SecurityDefinitions class header
 ************************************************************/

#endif
