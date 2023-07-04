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

#ifndef GPUI_COMMENT_DEFINITION_RESOURCES_H
#define GPUI_COMMENT_DEFINITION_RESOURCES_H

#include <string>
#include <vector>

class Presentation;

namespace comments
{

/*!
 * \brief The CommentDefinitionResources class
 */
class CommentDefinitionResources
{
public:
    /*!
     * \brief revision  The revision number.
     */
    uint32_t revision{};
    /*!
     * \brief schemaVersion  The version number of the applicable schema.
     */
    uint32_t schemaVersion{};
    /*!
     * \brief stringTable Table of the strings.
     */
    std::vector<std::pair<std::string, std::string>> stringTable{};

    /*!
     * \brief presentationTable List of pointers to presentation elements.
     */
    std::vector<Presentation*> presentationTable{};
};

}

#endif//GPUI_COMMENT_DEFINITION_RESOURCES_H
