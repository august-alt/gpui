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

#ifndef GPUI_PRESENTATIONWIDGETVISITOR_H
#define GPUI_PRESENTATIONWIDGETVISITOR_H

#include "../model.h"

namespace model
{
    namespace presentation
    {
        class CheckBox;
        class ComboBox;
        class DecimalTextBox;
        class DropdownList;
        class ListBox;
        class LongDecimalTextBox;
        class MultiTextBox;
        class Text;
        class TextBox;

        /*!
         * \brief The PresentationWidgetVisitor class
         */
        class GPUI_MODEL_EXPORT PresentationWidgetVisitor {
        public:
            virtual void visitCheckBox(const CheckBox& widget) const = 0;

            virtual void visitComboBox(const ComboBox& widget) const = 0;

            virtual void visitDecimalTextBox(const DecimalTextBox& widget) const = 0;

            virtual void visitDropdownList(const DropdownList& widget) const = 0;

            virtual void visitListBox(const ListBox& widget) const = 0;

            virtual void visitLongDecimalTextBox(const LongDecimalTextBox& widget) const = 0;

            virtual void visitMultiTextBox(const MultiTextBox& widget) const = 0;

            virtual void visitText(const Text& widget) const = 0;

            virtual void visitTextBox(const TextBox& widget) const = 0;
        };
    }
}

#endif // GPUI_PRESENTATIONWIDGETVISITOR_H
