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

#ifndef GPUI_LISTBOX_H
#define GPUI_LISTBOX_H

#include "../model.h"
#include "presentationwidget.h"
#include "presentationwidgetvisitor.h"

namespace model
{
    namespace presentation
    {
        /*!
         * \class ListBox
         * \brief The ListBox class represents a listbox display element.
         *
         * \ingroup model
         * \ingroup presentation
         */
        class GPUI_MODEL_EXPORT ListBox : public PresentationWidget
        {
        public:
            ListBox(Presentation* presentation)
                : PresentationWidget(presentation)
            {

            }

            virtual void accept(const PresentationWidgetVisitor& visitor) const override {
                visitor.visitListBox(*this);
            }
        };
    }
}

#endif // GPUI_LISTBOX_H
