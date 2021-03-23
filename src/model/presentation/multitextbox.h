/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd.
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

#include "../model.h"
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
        class GPUI_MODEL_EXPORT MultiTextBox : public PresentationWidget
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

            MultiTextBox(Presentation* presentation)
                : PresentationWidget(presentation)
            {

            }

            virtual void accept(const PresentationWidgetVisitor& visitor) const override {
                visitor.visitMultiTextBox(*this);
            }
        };
    }
}

#endif // GPUI_MULTITEXTBOX_H
