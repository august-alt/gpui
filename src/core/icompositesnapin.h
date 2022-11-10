/***********************************************************************************************************************
**
** Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>
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

#ifndef _ICOMPOSITESNAPIN_H
#define _ICOMPOSITESNAPIN_H

#include "core.h"

#include "isnapin.h"

namespace gpui
{
/**
 * @brief Composite snap-in requires list of dependencies to operate.
 */
class GPUI_CORE_EXPORT ICompositeSnapIn : public virtual ISnapIn
{
public:
    virtual QMap<QString, QVersionNumber> getDependencies() const = 0;
};

} // namespace gpui

#endif //_ICOMPOSITESNAPIN_H
