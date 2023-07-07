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

#ifndef GPUI_POLICY_H
#define GPUI_POLICY_H

#include "policyitem.h"
#include "policytype.h"
#include "registryvalue.h"

#include <memory>
#include <string>
#include <vector>

namespace model
{
namespace admx
{
class PolicyElement;

/*!
 * \brief The Policy class corresponds to a single Group Policy setting.
 */
class Policy
{
public:
    /*!
     * \brief name Specifies a logical name to use for a specific supported application and revision.
     */
    std::string name{};

    /*!
     * \brief namespace Namespace of the policy.
     */
    std::string namespace_{};

    /*!
     * \brief displayName The reference to the policy setting title text string located in the string
     * table of the .adml file.
     */
    std::string displayName{};

    /*!
     * \brief explainText Explain or Help text associated with a specific policy setting.
     */
    std::string explainText{};

    /*!
     * \brief presentation A reference to a policy presentation in the localized presentation section/table.
     */
    std::unique_ptr<std::string> presentation{};

    /*!
     * \brief key The registry key location under which the registry value will be created.
     */
    std::string key{};

    /*!
     * \brief valueName The registry value that will be configured for this specific policy element.
     */
    std::string valueName{};

    /*!
     * \brief policyType Identifies whether the policy will be located under a single node
     * (computer or user) or both nodes.
     */
    PolicyType policyType = PolicyType::Machine;

    /*!
     * \brief parentCategory Reference to parent of the current category.
     */
    std::string parentCategory{};

    /*!
     * \brief seeAlso The seeAlso element is a descriptive phrase that can be used to reference related
     * categories or policy settings.
     */
    std::vector<std::string> seeAlso{};

    /*!
     * \brief supportedOn A reference to a supported product definition, either a single version
     * or a complex definition.
     */
    std::string supportedOn{};

    /*!
     * \brief elements One of five parameters types that can be set in a policy setting.
     */
    std::vector<std::unique_ptr<PolicyElement>> elements{};

    /*!
     * \brief enabledList A set of decimal or string values and registry keys
     * representing the enabled state of the policy element.
     */
    std::vector<std::unique_ptr<PolicyItem>> enabledList{};

    /*!
     * \brief disabledList A set of decimal or string values and registry keys
     * representing the disabled state of the policy element.
     */
    std::vector<std::unique_ptr<PolicyItem>> disabledList{};

    /*!
     * \brief enabledValue A decimal or string value representing the enabled
     * value of the registry valueName element.
     */
    std::unique_ptr<AbstractRegistryValue> enabledValue = nullptr;

    /*!
     * \brief disabledValue A decimal or string value representing the disabled
     * value of the registry valueName element.
     */
    std::unique_ptr<AbstractRegistryValue> disabledValue = nullptr;

    Policy() = default;

    virtual ~Policy() = default;
};
} // namespace admx
} // namespace model

#endif // GPUI_POLICY_H
