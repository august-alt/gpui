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

#ifndef GPUI_LOCALIZATION_RESOURCE_REFERENCE_H
#define GPUI_LOCALIZATION_RESOURCE_REFERENCE_H

#include <string>
#include <vector>

namespace comments
{

/*!
 * \brief The LocalizationResourceReference class  An .admx implementation of the PolicyDefinitionResources.
 * The minimum version of the .adml file is required.
 */
class LocalizationResourceReference
{
public:
    /*!
     * \brief minRequiredRevision  Minimal version of required revision of a policy.
     */
    std::string minRequiredRevision;

    /*!
     * \brief fallbackCulture  Language to fallback to.
     */
    std::string fallbackCulture = "en-US";

    /*!
     * \brief stringTable Table of the strings.
     */
    std::vector<std::string> stringTable;
};

}

#endif//GPUI_LOCALIZATION_RESOURCE_REFERENCE_H
