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

#ifndef GPUI_DECIMALTEXTBOX_H
#define GPUI_DECIMALTEXTBOX_H

#include "../administrativetemplates.h"

#include <string>

#include "presentationwidget.h"
#include "presentationwidgetvisitor.h"

namespace model
{
namespace presentation
{
/*!
 * \class DecimalTextBox
 * \brief The DecimalTextBox class represents a text box with or without a spin control for entering decimal numbers.
 *
 * \ingroup model
 * \ingroup presentation
 */
class DecimalTextBox : public PresentationWidget
{
public:
    /*!
     * \brief defaultValue Specifies a default value.
     */
    unsigned int defaultValue = 1;

    /*!
     * \brief spin If true, create a spin control; otherwise, create a text box for numeric entry.
     */
    bool spin = true;

    /*!
     * \brief spinStep The increment of change for the spin control.
     */
    unsigned int spinStep = 1;

    DecimalTextBox(Presentation *presentation)
        : PresentationWidget(presentation)
    {}

    virtual bool accept(const PresentationWidgetVisitor &visitor) override { return visitor.visit(*this); }

    virtual std::string acceptCheck(const PresentationWidgetVisitor &visitor) override { return visitor.check(*this); }
};
} // namespace presentation
} // namespace model

#endif // GPUI_DECIMALTEXTBOX_H
