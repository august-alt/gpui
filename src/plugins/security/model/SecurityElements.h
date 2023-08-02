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
#ifndef SECURITY_SECURITYELEMENTS_H
#define SECURITY_SECURITYELEMENTS_H

/************************************************************
 SecurityElements class header
 ************************************************************/

#include "security/Pkg_security.h"

#include "EcorePrimitiveTypes/Pkg_EcorePrimitiveTypes.h"
#include "security/BooleanElement.h"
#include "security/DecimalElement.h"
#include "security/EnumerationElement.h"
#include "security/ListElement.h"
#include "security/LongDecimalElement.h"
#include "security/MultiTextElement.h"
#include "security/TextElement.h"

namespace security {

/************************************************************/
/**
 * 
 */
class SecurityElements {
public:

	/**
	 * 
	 */
	BooleanElement bool[];

	/**
	 * 
	 */
	DecimalElement decimal[];

	/**
	 * 
	 */
	TextElement text[];

	/**
	 * 
	 */
	EnumerationElement enum[];

	/**
	 * 
	 */
	ListElement list[];

	/**
	 * 
	 */
	LongDecimalElement longDecimal[];

	/**
	 * 
	 */
	MultiTextElement multiText[];
};
/************************************************************/
/* External declarations (package visibility)               */
/************************************************************/

/* Inline functions                                         */

} // of namespace security

/************************************************************
 End of SecurityElements class header
 ************************************************************/

#endif
