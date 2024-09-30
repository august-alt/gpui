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

#ifndef SECURITY_SECURITY_DESCRIPTOR_EDITOR_H
#define SECURITY_SECURITY_DESCRIPTOR_EDITOR_H

#include "dataelement.h"

namespace security
{

/*!
 * \class SecurityDescriptorEditor
 * \brief Represents a security descriptor editor display element.
 */
class SecurityDescriptorEditor: public DataElement
{
public:
    /*!
     * \brief label Label of the security editor button.
     */
    std::string label{};
};

} // of namespace security

#endif // SECURITY_SECURITY_DESCRIPTOR_EDITOR_H
