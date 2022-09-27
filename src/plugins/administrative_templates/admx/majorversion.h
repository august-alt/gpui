/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd. <org@basealt.ru>
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

#ifndef GPUI_MAJORVERSION_H
#define GPUI_MAJORVERSION_H

#include "minorversion.h"

#include <vector>

namespace model
{
namespace admx
{
/*!
 * \brief The MajorVersion class A major version of a product.
 */
class MajorVersion : public MinorVersion
{
public:
    /*!
     * \brief minorVersion A minor version of a product.
     */
    std::vector<MinorVersion> minorVersion{};
};
} // namespace admx
} // namespace model

#endif // GPUI_MAJORVERSION_H
