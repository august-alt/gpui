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

#ifndef SECURITY_SECURITY_PRESENTATION_RESOURCES_H
#define SECURITY_SECURITY_PRESENTATION_RESOURCES_H

#include "securitypresentation.h"

#include <map>
#include <string>

namespace security
{

/*!
 * \class SecurityPresentationResources
 * \brief Root presentation element.
 */
class SecurityPresentationResources
{
public:
    /*!
     * \brief displayName The localized friendly name of the policy settings file.
     * Unsupported by current Group Policy tools.
     */
    std::string displayName{};

    /*!
     * \brief description The localized description of policy settings contained in an .adml file.
     * Unsupported by current Group Policy tools.
     */
    std::string description{};

    /*!
     * \brief annotation A comment string for .adml files.
     * Strings added to this tag will not be processed by Group Policy tools.
     */
    std::vector<std::string> annotation{};

    /*!
     * \brief stringTable A table of localized strings.
     */
    std::map<std::string, std::string> stringTable{};

    /*!
     * \brief presentationTable A table of presentation elements representing policy setting parameters.
     */
    std::vector<std::pair<std::string, std::shared_ptr<SecurityPresentation>>> presentationTable{};
};

} // of namespace security

#endif // SECURITY_SECURITY_PRESENTATION_RESOURCES_H
