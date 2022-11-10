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

#ifndef GPUI_COMBOBOX_H
#define GPUI_COMBOBOX_H

#include "../administrativetemplates.h"

#include "presentationwidget.h"
#include "presentationwidgetvisitor.h"

#include <string>
#include <vector>

namespace model
{
namespace presentation
{
/*!
 * \class ComboBox
 * \brief Represents a combo box display element with default/suggested entries.
 *
 * \ingroup model
 * \ingroup presentation
 */
class ComboBox : public PresentationWidget
{
public:
    /*!
     * \brief refId A reference Id.
     */
    std::string refId{};

    /*!
     * \brief label Text associated with the input box to provide prompt text.
     */
    std::string label{};

    /*!
     * \brief defaultValue Specifies a default value. This can be used for either string or numeric data.
     */
    std::string defaultValue{};

    /*!
     * \brief suggestions A suggested value to be placed in the drop-down list.
     * Multiple suggestion elements result in multiple suggestions.
     */
    std::vector<std::string> suggestions{};

    ComboBox(Presentation *presentation)
        : PresentationWidget(presentation)
    {}

    virtual void accept(const PresentationWidgetVisitor &visitor) override { visitor.visit(*this); }
};
} // namespace presentation
} // namespace model

#endif // GPUI_COMBOBOX_H
