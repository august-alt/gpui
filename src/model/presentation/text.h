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

#ifndef GPUI_TEXT_H
#define GPUI_TEXT_H

#include "../model.h"
#include "presentationwidget.h"
#include "presentationwidgetvisitor.h"

#include <string>

namespace model
{
    namespace presentation
    {
        /*!
         * \class Text
         * \brief The Text class The text element, when used within the presentation element,
         * is a localized string displayed within the parameter box.
         *
         * \ingroup model
         * \ingroup presentation
         */
        class Text : public PresentationWidget
        {
        public:
            std::string content;

            Text(Presentation* presentation)
                : PresentationWidget(presentation)
            {

            }

            virtual void accept(const PresentationWidgetVisitor& visitor) override {
                visitor.visit(*this);
            }
        };
    }
}

#endif // GPUI_TEXT_H
