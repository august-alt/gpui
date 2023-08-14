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

#ifndef GPUI_MULTITEXTBOX_H
#define GPUI_MULTITEXTBOX_H

#include "../administrativetemplates.h"

#include "presentationwidget.h"
#include "presentationwidgetvisitor.h"

#include <string>

namespace model
{
namespace presentation
{
/*!
* \brief The MultiTextBox class represents a multi-line textbox display element.
* Can be associated with a multiTextElement.
*
* \ingroup model
* \ingroup presentation
*/
class MultiTextBox : public PresentationWidget
{
public:
    /*!
     * \brief refId A reference Id.
     */
    std::string refId;

    /*!
     * \brief showAsDialog If we need to display multitext as a dialog.
     */
    bool showAsDialog = false;

    /*!
     * \brief defaultHeight Deafult height of the widget.
     */
    unsigned int defaultHeight = 3;

    /*!
     * \brief label Text associated with the text box.
     */
    std::string label;

    MultiTextBox(Presentation *presentation)
        : PresentationWidget(presentation)
        , refId()
        , label()
    {}

    virtual bool accept(const PresentationWidgetVisitor &visitor) override { return visitor.visit(*this); }

    virtual std::string acceptCheck(const PresentationWidgetVisitor &visitor) override { return visitor.check(*this); }
};
} // namespace presentation
} // namespace model

#endif // GPUI_MULTITEXTBOX_H
