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

#ifndef GPUI_POLICYboolELEMENT_H
#define GPUI_POLICYboolELEMENT_H

#include "policyelement.h"
#include "policyenumelement.h"

#include <any>
#include <string>

namespace model
{
namespace admx
{
// TODO: Add key to vector of true list.
/*!
 * \brief A choice element in a policy with associated values for the true and false cases.
 */
class PolicyBoolElement : public PolicyElement
{
public:
    /*!
     * \brief trueValue Value associated with true.
     */
    std::unique_ptr<EnumValue> trueValue {};

    /*!
     * \brief falseValue Value associated with false.
     */
    std::unique_ptr<EnumValue> falseValue {};

    /*!
     * \brief trueList List of values assicated with true.
     */
    std::vector<std::pair<std::string, std::unique_ptr<EnumValue>>> trueList {};

    /*!
     * \brief trueList List of values assicated with false.
     */
    std::vector<std::pair<std::string, std::unique_ptr<EnumValue>>> falseList {};

    registry::RegistryEntryType getRegistryEntryType() const override { return registry::REG_DWORD; }
};
} // namespace admx
} // namespace model

#endif // GPUI_POLICYboolELEMENT_H
