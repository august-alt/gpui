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

#ifndef GPUI_CHECKBOX_H
#define GPUI_CHECKBOX_H

#include "../administrativetemplates.h"

#include "presentationwidget.h"
#include "presentationwidgetvisitor.h"

#include <string>

namespace model
{
namespace presentation
{
/*!
 * \class CheckBox
 * \brief The CheckBox element represents a check box parameter.
 *
 * \ingroup model
 * \ingroup presentation
 *
 * The CheckBox element must be associated with a boolean element defined in the elements element.
 */
class CheckBox : public PresentationWidget
{
public:
    /*!
     * \brief defaultChecked True if the default value is displayed with a checkmark.
     */
    bool defaultChecked = false;

    /*!
     * \brief label Text associated with the check box.
     */
    std::string label{};

    CheckBox(Presentation *presentation)
        : PresentationWidget(presentation)
    {}

    virtual void accept(const PresentationWidgetVisitor &visitor) override { visitor.visit(*this); }
};
} // namespace presentation
} // namespace model

#endif // GPUI_CHECKBOX_H
