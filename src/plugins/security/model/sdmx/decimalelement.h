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

#ifndef SECURITY_DECIMAL_ELEMENT_H
#define SECURITY_DECIMAL_ELEMENT_H

#include "security.h"

#include "securityelement.h"

namespace security
{

/*!
 * \brief The DecimalElement class
 */
class DecimalElement: public SecurityElement
{
public:

    /*!
     *
     */
    bool required { false };

    /*!
     *
     */
    bool soft { false };

    /*!
     *
     */
    bool storeAsText { false };

    /*!
     *
     */
    uint32_t minValue { std::numeric_limits<uint32_t>::min() };

    /*!
     *
     */
    uint32_t maxValue { std::numeric_limits<uint32_t>::max() };
};

} // of namespace security

#endif // SECURITY_DECIMAL_ELEMENT_H
