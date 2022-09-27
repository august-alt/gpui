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

#ifndef _SnapInManager_H
#define _SnapInManager_H

#include "core.h"

#include "isnapin.h"
#include "isnapinmanager.h"

namespace gpui
{
class SnapInManagerPrivate;

/**
 * @brief The SnapInManager class provides implementation of ISnapInManager interface.
 */
class GPUI_CORE_EXPORT SnapInManager final : public ISnapInManager
{
public:
    SnapInManager();
    ~SnapInManager();

    void addSnapIn(ISnapIn *snapIn) override final;

    void removeSnapIn(ISnapIn *snapIn) override final;

    std::vector<ISnapIn *> getSnapIns() const override final;

    void clear() override final;

private:
    SnapInManager(const SnapInManager &) = delete;            // copy ctor
    SnapInManager(SnapInManager &&)      = delete;            // move ctor
    SnapInManager &operator=(const SnapInManager &) = delete; // copy assignment
    SnapInManager &operator=(SnapInManager &&) = delete;      // move assignment

private:
    SnapInManagerPrivate *d;
};

} // namespace gpui

#endif //_SnapInManager_H
