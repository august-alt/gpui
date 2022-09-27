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

#ifndef GPUI_POLICYLISTELEMENT_H
#define GPUI_POLICYLISTELEMENT_H

#include "policyelement.h"

#include <string>

namespace model
{
namespace admx
{
/*!
 * \brief The PolicyListElement class sets a list of different registry subkeys when a policy setting is enabled.
 */
class PolicyListElement : public PolicyElement
{
public:
    /*!
     * \brief A prefix for value.
     */
    std::string valuePrefix{};

    /*!
     * \brief additive If this option is specified true, existing subkeys are not deleted,
     * and the values set in the list box are appended to the existing subkeys.
     */
    bool additive = false;

    /*!
     * \brief expandable The registry subkey value will be created as an expandable string type (REG_EXPAND_SZ)
     * instead of a string type (REG_SZ).
     */
    bool expandable = false;

    /*!
     * \brief explicitValue If this option is specified true the user must specify the registry subkey value
     * and the registry subkey name. The list box shows two columns, one for the name and one for the data.
     */
    bool explicitValue = false;

    registry::RegistryEntryType getRegistryEntryType() const override
    {
        return expandable ? registry::REG_SZ : registry::REG_EXPAND_SZ;
    }
};
} // namespace admx
} // namespace model

#endif // GPUI_POLICYLISTELEMENT_H
