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

#ifndef GPUI_TEXTBOX_H
#define GPUI_TEXTBOX_H

#include "../administrativetemplates.h"

#include "presentationwidget.h"
#include "presentationwidgetvisitor.h"

#include <string>

namespace model
{
namespace presentation
{
/*!
 * \class TextBox
 * \brief The TextBox class represents a text box display element.
 *
 * \ingroup model
 * \ingroup presentation
 */
class TextBox : public PresentationWidget
{
public:
    /*!
     * \brief refId A reference Id.
     */
    std::string refId;

    /*!
     * \brief label Text associated with the input box to provide prompt text.
     */
    std::string label;

    /*!
     * \brief defaultValue Specifies a default value. This can be used for either string or numeric data.
     */
    std::string defaultValue;

    TextBox(Presentation *presentation)
        : PresentationWidget(presentation)
        , refId()
        , label()
        , defaultValue()
    {}

    virtual bool accept(const PresentationWidgetVisitor &visitor) override { return visitor.visit(*this); }

    virtual std::string acceptCheck(const PresentationWidgetVisitor &visitor) override { return visitor.check(*this); }
};
} // namespace presentation
} // namespace model

#endif // GPUI_TEXTBOX_H
