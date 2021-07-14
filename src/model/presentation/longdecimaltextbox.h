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

#ifndef GPUI_LONGDECIMALTEXTBOX_H
#define GPUI_LONGDECIMALTEXTBOX_H

#include "../model.h"
#include "presentationwidget.h"
#include "presentationwidgetvisitor.h"

#include <string>

namespace model
{
    namespace presentation
    {
        /*!
         * \class LongDecimalTextBox
         * \brief The LongDecimalTextBox class represents a text box with or without a spin
         * control for entering 64-bit decimal numbers.
         * Can be associated with either a LongDecimalElement or TextElement.
         *
         * \ingroup model
         * \ingroup presentation
         */
        class GPUI_MODEL_EXPORT LongDecimalTextBox : public PresentationWidget
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
             * \brief spinStep the increment of change for the spin control.
             */
            unsigned int spinStep = 1;

            /*!
             * \brief label Text associated with the text box.
             */
            std::string label;

            LongDecimalTextBox(Presentation* presentation)
                : PresentationWidget(presentation)
            {

            }

            virtual void accept(const PresentationWidgetVisitor& visitor) const override {
                visitor.visitLongDecimalTextBox(*this);
            }
        };
    }
}

#endif // GPUI_LONGDECIMALTEXTBOX_H
