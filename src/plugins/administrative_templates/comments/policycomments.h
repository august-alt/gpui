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

#ifndef GPUI_POLICY_COMMENTS_H
#define GPUI_POLICY_COMMENTS_H

#include "comment.h"
#include "localizationresourcereference.h"
#include "policynamespace.h"

#include <memory>
#include <vector>


namespace comments
{

/*!
 * \brief The PolicyComments class This outermost (document) element may not be contained by any other elements.
 */
class PolicyComments
{
public:
    /**
     * @brief policyNamespaces  Root element for <using> elements.
     */
    PolicyNamespace policyNamespaces{};

    /*!
     * \brief revision The revision number.
     */
    uint32_t revision{0};

    /*!
     * \brief schemaVersion The version number of the applicable schema.
     */
    uint32_t schemaVersion{0};
    /*!
     * \brief comments  List of comments.
     */
    std::vector<Comment> comments{};

    /*!
     * \brief resources Resources.
     */
    std::unique_ptr<LocalizationResourceReference> resources{nullptr};
};

}

#endif  //_POLICYCOMMENTS_H
