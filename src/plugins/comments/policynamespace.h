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

#ifndef GPUI_POLICY_NAMESPACE_H
#define GPUI_POLICY_NAMESPACE_H

#include "policynamespaceassociation.h"

#include <memory>
#include <vector>

namespace comments
{

/*!
 * \brief The PolicyNamespace class Root element for <using> elements.
 */
class PolicyNamespace
{
public:
    /*!
     * \brief usingNamespace A set of PolicyNamespaceAssociation types referenced by an .admx file.
     */
    std::vector<PolicyNamespaceAssociation> using_{};
};

}

#endif  //GPUI_POLICY_NAMESPACE_H
