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
#ifndef SECURITY_SECURITYVALUE_H
#define SECURITY_SECURITYVALUE_H

/************************************************************
 SecurityValue class header
 ************************************************************/

#include "security/Pkg_security.h"

#include "security/DecimalType.h"
#include "security/LongDecimalType.h"

namespace security {

/************************************************************/
/**
 * 
 */
class SecurityValue {
public:

	/**
	 * 
	 */
	DecimalType decimal;

	/**
	 * 
	 */
	LongDecimalType longDecimal;

	/**
	 * 
	 */
	SecurityType std::string;

};
/************************************************************/
/* External declarations (package visibility)               */
/************************************************************/

/* Inline functions                                         */

} // of namespace security

/************************************************************
 End of SecurityValue class header
 ************************************************************/

#endif
