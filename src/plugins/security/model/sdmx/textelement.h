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

#ifndef SECURITY_TEXT_ELEMENT_H
#define SECURITY_TEXT_ELEMENT_H

#include "security.h"

#include "securityelement.h"

namespace security
{

/**
 * \class TextElement
 * \brief Describes a single line text element in a policy.
 *
 * \ingroup model
 */
class TextElement: public SecurityElement
{
public:

    /*!
     * \brief required If this element is required.
     */
    bool required { false };

    /*!
     * \brief maxLength Maximum length of text element. If text exceeds maximum length it will be truncated.
     */
    uint32_t maxLength { 1024 };

    /*!
     * \brief soft If previous value should be replaced.
     */
    bool soft { false };
};

} // of namespace security

#endif // SECURITY_TEXT_ELEMENT_H
