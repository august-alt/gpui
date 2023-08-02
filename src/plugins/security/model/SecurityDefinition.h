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
#ifndef SECURITY_SECURITYDEFINITION_H
#define SECURITY_SECURITYDEFINITION_H

/************************************************************
 SecurityDefinition class header
 ************************************************************/

#include "security/Pkg_security.h"

#include "security/CategoryReference.h"
#include "security/SecurityElements.h"
#include "security/SecurityValue.h"
#include "security/SecurityValueList.h"

#include <string>

namespace security {

/************************************************************/
/**
 * 
 */
class SecurityDefinition {
public:

	/**
	 * 
	 */
	std::string name;

	/**
	 * 
	 */
	std::string displayName;

	/**
	 * 
	 */
	std::string explainText;

	/**
	 * 
	 */
	std::string sectionName;

	/**
	 * 
	 */
	std::string propertyName;

	/**
	 * 
	 */
	SecurityType securityType;

	/**
	 * 
	 */
	CategoryReference parentCategory;

	/**
	 * 
	 */
	SecurityElements elements;

	/**
	 * 
	 */
	SecurityValue enabledValue;

	/**
	 * 
	 */
	SecurityValue disabledValue;

	/**
	 * 
	 */
	SecurityValueList enabledList;

	/**
	 * 
	 */
	SecurityValueList disabledList;

};
/************************************************************/
/* External declarations (package visibility)               */
/************************************************************/

/* Inline functions                                         */

} // of namespace security

/************************************************************
 End of SecurityDefinition class header
 ************************************************************/

#endif
