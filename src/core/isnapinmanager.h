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

#ifndef _ISNAPINMANAGER_H
#define _ISNAPINMANAGER_H

#include "core.h"

#include <vector>

#include "isnapin.h"

namespace gpui
{
/**
 * @brief class ISnapInManager Manages all the snap-ins in an application.
 */
class GPUI_CORE_EXPORT ISnapInManager
{
public:
    virtual ~ISnapInManager() = default;

    /**
     * @brief addSnapIn Adds a snap-in to the manager.
     * @param snapIn Snap-In to add.
     */
    virtual void addSnapIn(ISnapIn *snapIn) = 0;

    //
    /**
     * @brief removeSnapIn Removes a snap-in from the manager.
     * @param snapIn Snap-In to remove.
     */
    virtual void removeSnapIn(ISnapIn *snapIn) = 0;

    /**
     * @brief getSnapIns Get a list of all snap-ins associated with this manager.
     * @return List of all snap-ins.
     */
    virtual std::vector<ISnapIn *> getSnapIns() const = 0;

    /**
     * @brief Unload all the snap-ins from the manager.
     */
    virtual void clear() = 0;
};

} // namespace gpui

#endif //_ISNAPINMANAGER_H
