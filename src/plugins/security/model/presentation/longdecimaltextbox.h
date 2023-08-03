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

#ifndef SECURITY_LONG_DECIMAL_TEXTBOX_H
#define SECURITY_LONG_DECIMAL_TEXTBOX_H

#include "dataelementcontent.h"

namespace security {

/**
 * \class LongDecimalTextBox
 * \brief Represents a text box with or without a spin control for entering long decimal numbers.
 *
 * \ingroup security
 * \ingroup presentation
 */
class LongDecimalTextBox: public DataElementContent
{
public:

    /*!
     * \brief defaultValue
     */
    uint64_t defaultValue;

    /*!
     * \brief spinBox
     */
    bool spinBox { false };

    /*!
     * \brief spinStep
     */
    uint64_t spinStep {1};

};

} // of namespace security

#endif // SECURITY_LONG_DECIMAL_TEXTBOX_H
