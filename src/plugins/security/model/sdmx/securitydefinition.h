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

#ifndef SECURITY_SECURITY_DEFINITION_H
#define SECURITY_SECURITY_DEFINITION_H

#include "security.h"

#include "securityelements.h"
#include "securityvalue.h"
#include "securityvaluelist.h"

#include <memory>
#include <string>
#include <vector>

namespace security
{

/*!
   \class SecurityDefinition
 * \brief The SecurityDefinition class
 */
class SecurityDefinition
{
public:
    /*!
     * \brief name
     */
    std::string name {};

    /*!
     * \brief displayName
     */
    std::string displayName {};

    /*!
     * \brief explainText
     */
    std::string explainText {};

    /*!
     * \brief sectionName
     */
    std::unique_ptr<std::string> sectionName {};

    /*!
     * \brief propertyName
     */
    std::unique_ptr<std::string> propertyName {};

    /*!
     * \brief securityType
     */
    SecurityType securityType { SecurityType::Machine };

    /*!
     * \brief parentCategory
     */
    std::string parentCategory {};

    /*!
     * \brief elements
     */
    std::unique_ptr<SecurityElements> elements {};

    /*!
     * \brief enabledValue
     */
    std::unique_ptr<SecurityValue> enabledValue {};

    /*!
     * \brief disabledValue
     */
    std::unique_ptr<SecurityValue> disabledValue {};

    /*!
     * \brief enabledList
     */
    std::unique_ptr<SecurityValueList> enabledList;

    /*!
     * \brief disabledList
     */
    std::unique_ptr<SecurityValueList> disabledList;
};

} // of namespace security

#endif // SECURITY_SECURITY_DEFINITION_H
