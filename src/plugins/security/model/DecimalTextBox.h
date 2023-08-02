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
#ifndef SECURITY_DECIMALTEXTBOX_H
#define SECURITY_DECIMALTEXTBOX_H

/************************************************************
 DecimalTextBox class header
 ************************************************************/

#include "security/Pkg_security.h"

#include "security/DataElementContent.h"

namespace security {

/************************************************************/
/**
 * Represents a text box with or without a spin control for entering decimal numbers.
 */
class DecimalTextBox: public DataElementContent {
public:

	/**
	 * 
	 */
	unsigned int defaultValue;

	/**
	 * 
	 */
	bool spinBox;

	/**
	 * 
	 */
	unsigned int spinStep;

};
/************************************************************/
/* External declarations (package visibility)               */
/************************************************************/

/* Inline functions                                         */

} // of namespace security

/************************************************************
 End of DecimalTextBox class header
 ************************************************************/

#endif
