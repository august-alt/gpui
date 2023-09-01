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

#ifndef GPUI_PRESENTATIONWIDGET_H
#define GPUI_PRESENTATIONWIDGET_H

#include "../administrativetemplates.h"

#include <string>
#include <functional>

namespace model
{
namespace presentation
{
class Presentation;
class PresentationWidgetVisitor;

/*!
 * \brief The PresentationWidget class base class for all widgets supported by ADMX/ADML format.
 */
class GPUI_ADMINISTRATIVE_TEMPLATES_EXPORT PresentationWidget
{
public:
    /*!
     * \brief label Text associated with the input box to provide prompt text.
     */
    std::string label{};

    /*!
     * \brief requirementsMet returns False only when field is required and empty.
     */
    std::function<bool()> requirementsMet = [](){ return true; };

    explicit PresentationWidget(Presentation *parent);

    virtual ~PresentationWidget() = default;

    virtual bool accept(const PresentationWidgetVisitor &visitor) = 0;

    virtual std::string acceptCheck(const PresentationWidgetVisitor &visitor) = 0;

private:
    PresentationWidget(const PresentationWidget &) = delete;            // copy ctor
    PresentationWidget(PresentationWidget &&)      = delete;            // move ctor
    PresentationWidget &operator=(const PresentationWidget &) = delete; // copy assignment
    PresentationWidget &operator=(PresentationWidget &&) = delete;      // move assignment

protected:
    Presentation *parent;
};
} // namespace presentation
} // namespace model

#endif // GPUI_PRESENTATIONWIDGET_H
