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

#include "../administrativetemplates.h"

#include <string>

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
class GPUI_ADMINISTRATIVE_TEMPLATES_EXPORT PresentationWidgetVisitor
{
public:
    virtual ~PresentationWidgetVisitor() = default;

    virtual bool visit(CheckBox &widget) const = 0;

    virtual bool visit(ComboBox &widget) const = 0;

    virtual bool visit(DecimalTextBox &widget) const = 0;

    virtual bool visit(DropdownList &widget) const = 0;

    virtual bool visit(ListBox &widget) const = 0;

    virtual bool visit(LongDecimalTextBox &widget) const = 0;

    virtual bool visit(MultiTextBox &widget) const = 0;

    virtual bool visit(Text &widget) const = 0;

    virtual bool visit(TextBox &widget) const = 0;


    virtual std::string check(CheckBox &widget) const = 0;

    virtual std::string check(ComboBox &widget) const = 0;

    virtual std::string check(DecimalTextBox &widget) const = 0;

    virtual std::string check(DropdownList &widget) const = 0;

    virtual std::string check(ListBox &widget) const = 0;

    virtual std::string check(LongDecimalTextBox &widget) const = 0;

    virtual std::string check(MultiTextBox &widget) const = 0;

    virtual std::string check(Text &widget) const = 0;

    virtual std::string check(TextBox &widget) const = 0;

};
} // namespace presentation
} // namespace model

#endif // GPUI_PRESENTATIONWIDGETVISITOR_H
