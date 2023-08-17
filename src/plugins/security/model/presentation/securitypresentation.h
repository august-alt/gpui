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

#ifndef SECURITY_SECURITY_PRESENTATION_H
#define SECURITY_SECURITY_PRESENTATION_H

#include "presentationelements.h"

#include <memory>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace security
{
// the variant to visit
typedef ::std::variant<std::shared_ptr<DecimalTextBox>,
                       std::shared_ptr<TextBox>,
                       std::shared_ptr<CheckBox>,
                       std::shared_ptr<ComboBox>,
                       std::shared_ptr<DropdownList>,
                       std::shared_ptr<LongDecimalTextBox>,
                       std::shared_ptr<MultiTextBox>,
                       std::shared_ptr<GroupBox>,
                       std::shared_ptr<ListBox>,
                       std::shared_ptr<Comment>,
                       std::shared_ptr<Text> > PresentationElement;

/*!
 * \class SecurityPresentation
 * \brief Root presentation element.
 *
 * \ingroup security
 * \ingroup presentation
 */
class SecurityPresentation: public PresentationElements
{
public:
    /*!
     * \brief id Id of current presentation element.
     */
    std::string id{};

    /*!
     * \brief text
     */
    std::string text{};

    /*!
     * \brief icon Icon associated with current presentation element.
     */
    std::string icon{};

    /*!
     * \brief widgets List of widgets, associated with their ids.
     */
    std::vector<std::pair<std::string, PresentationElement>> widgets{};
};

} // of namespace security

#endif // SECURITY_SECURITY_PRESENTATION_H
