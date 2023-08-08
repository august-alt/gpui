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

#ifndef SECURITY_PRESENTATION_ELEMENTS_H
#define SECURITY_PRESENTATION_ELEMENTS_H

#include "checkbox.h"
#include "combobox.h"
#include "comment.h"
#include "decimaltextbox.h"
#include "dropdownlist.h"
#include "groupbox.h"
#include "listbox.h"
#include "longdecimaltextbox.h"
#include "multitextbox.h"
#include "textbox.h"
#include "text.h"

#include <vector>

namespace security
{

/*!
 * \class PresentationElements
 * \brief The PresentationElements class
 */
class PresentationElements
{
public:

    /*!
     * \brief decimalTextBox
     */
    std::vector<DecimalTextBox> decimalTextBox {};

    /*!
     * \brief textBox
     */
    std::vector<TextBox> textBox{};

    /*!
     * \brief checkBox
     */
    std::vector<CheckBox> checkBox{};

    /*!
     * \brief comboBox
     */
    std::vector<ComboBox> comboBox{};

    /*!
     * \brief dropdownList
     */
    std::vector<DropdownList> dropdownList{};

    /*!
     * \brief longDecimalTextBox
     */
    std::vector<LongDecimalTextBox> longDecimalTextBox{};

    /*!
     * \brief multiTextBox
     */
    std::vector<MultiTextBox> multiTextBox{};

    /*!
     * \brief groupBox
     */
    std::vector<GroupBox> groupBox{};

    /*!
     * \brief listBox
     */
    std::vector<ListBox> listBox{};

    /*!
     * \brief comment
     */
    std::vector<Comment> comment{};

    /*!
     * \brief text
     */
    std::vector<Text> text{};
};

} // of namespace security

#endif // SECURITY_PRESENTATION_ELEMENTS_H
