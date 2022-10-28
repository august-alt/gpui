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

#include "snapinmanager.h"
#include "isnapin.h"

#include "pluginstorage.h"

#include <algorithm>

namespace gpui
{
class SnapInManagerPrivate
{
public:
    std::vector<ISnapIn *> snapIns{};
};

SnapInManager::SnapInManager()
    : d(new SnapInManagerPrivate())
{}

SnapInManager::~SnapInManager()
{
    for (auto &snapIn : d->snapIns)
    {
        if (snapIn)
        {
            delete snapIn;
            snapIn = nullptr;
        }
    }

    delete d;
}

void SnapInManager::addSnapIn(ISnapIn *snapIn)
{
    d->snapIns.push_back(snapIn);
}

void SnapInManager::removeSnapIn(ISnapIn *snapIn)
{
    auto element = std::find(d->snapIns.begin(), d->snapIns.end(), snapIn);
    if (element != d->snapIns.end())
    {
        d->snapIns.erase(element);
    }
}

std::vector<ISnapIn *> SnapInManager::getSnapIns() const
{
    return d->snapIns;
}

void SnapInManager::clear()
{
    d->snapIns.clear();
}

} // namespace gpui
