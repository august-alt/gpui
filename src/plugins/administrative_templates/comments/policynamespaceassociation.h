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

#ifndef GPUI_POLICY_NAMESPACE_ASSOCIATION_H
#define GPUI_POLICY_NAMESPACE_ASSOCIATION_H

#include <string>

namespace comments
{

/*!
 * \brief The PolicyNamespaceAssociation class A reference to a namespace used in the CMTX file.
 */
class PolicyNamespaceAssociation
{
public:
    /*!
     * \brief prefix An alias string used to refer to a namespace within the CMTX/ADMX file.
     */
    std::string prefix{};

    /*!
     * \brief namespace_ A namespace defined in an ADMX file.
     */
    std::string namespace_{};

    /*!
     * \brief PolicyNamespaceAssociation
     */
    PolicyNamespaceAssociation() noexcept
    {}

    /*!
     * \brief PolicyNamespaceAssociation
     * \param currentPrefix An alias string used to refer to a namespace within the CMTX/ADMX file.
     * \param currentNamespace A namespace defined in an ADMX file.
     */
    PolicyNamespaceAssociation(const std::string& currentPrefix,
                               const std::string& currentNamespace)
        : prefix(currentPrefix)
        , namespace_(currentNamespace)
    {}
};

}

#endif//GPUI_POLICY_NAMESPACE_ASSOCIATION_H
