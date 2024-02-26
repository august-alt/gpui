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

#ifndef GPUI_PRESENTATION_H
#define GPUI_PRESENTATION_H

#include "../administrativetemplates.h"

#include "presentationwidget.h"

#include <map>
#include <memory>
#include <vector>

namespace model
{
namespace presentation
{
/*!
 * \brief The Presentation class A presentation element representing parameters for a policy setting.
 *
 * \ingroup model
 * \ingroup presentation
 */
class Presentation
{
public:
    std::map<std::string, std::shared_ptr<PresentationWidget>> widgets{};
    std::vector<std::pair<std::shared_ptr<PresentationWidget>, std::string> > widgetsVector{};
};
} // namespace presentation
} // namespace model

#endif // GPUI_PRESENTATION_H
