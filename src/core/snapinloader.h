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

#ifndef SNAP_IN_LOADER_H
#define SNAP_IN_LOADER_H

#include "core.h"

#include <QDir>

namespace gpui
{
class ISnapInManager;

class SnapInLoaderPrivate;

class GPUI_CORE_EXPORT SnapInLoader
{
public:
    SnapInLoader(ISnapInManager *manager);
    ~SnapInLoader();

    void loadSnapIns(const QDir &snapInDirectory);

    void loadDefaultSnapIns();

private:
    SnapInLoader(const SnapInLoader &) = delete;            // copy ctor
    SnapInLoader(SnapInLoader &&)      = delete;            // move ctor
    SnapInLoader &operator=(const SnapInLoader &) = delete; // copy assignment
    SnapInLoader &operator=(SnapInLoader &&) = delete;      // move assignment

private:
    SnapInLoaderPrivate *d;
};

} // namespace gpui

#endif //SNAP_IN_LOADER_H
