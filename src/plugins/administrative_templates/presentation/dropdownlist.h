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

#ifndef GPUI_DROPDOWNLIST_H
#define GPUI_DROPDOWNLIST_H

#include "../administrativetemplates.h"

#include "presentationwidget.h"
#include "presentationwidgetvisitor.h"

#include <string>
#include <vector>

namespace model
{
namespace presentation
{
/*!
 * \class DropdownList
 * \brief The DropdownList class is a drop-down list display element, with which a user can pick one of the displayed entries.
 *
 * \ingroup model
 * \ingroup presentation
 */
class DropdownList : public PresentationWidget
{
public:
    /*!
     * \brief noSort
     */
    bool noSort = false;

    /*!
     * \brief defaultItem The numerical value identifying the default choice of the list of items.
     * The item list is numbered starting from 0.
     */
    unsigned int defaultItem = 0;

    /*!
     * \brief values Values of the drop-down list.
     */
    std::vector<std::string> values{};

    /*!
     * \brief refId A mapping to the element.
     */
    std::string refId{};

    DropdownList(Presentation *presentation)
        : PresentationWidget(presentation)
    {}

    virtual bool accept(const PresentationWidgetVisitor &visitor) override { return visitor.visit(*this); }

    virtual std::string acceptCheck(const PresentationWidgetVisitor &visitor) override { return visitor.check(*this); }
};
} // namespace presentation
} // namespace model

#endif // GPUI_DROPDOWNLIST_H
