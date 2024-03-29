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

#include "registryfile.h"

#include "../plugins/administrative_templates/registry/registry.h"

#include <algorithm>

using namespace model::registry;

namespace io
{
class RegistryFilePrivate
{
public:
    std::shared_ptr<Registry> registry = nullptr;
};

RegistryFile::RegistryFile()
    : d(new RegistryFilePrivate())
{
    d->registry = std::make_shared<Registry>();
}

RegistryFile::~RegistryFile()
{
    delete d;
}

void RegistryFile::setRegistry(std::shared_ptr<Registry> registryEntry)
{
    d->registry = registryEntry;
}

void RegistryFile::removeRegistry()
{
    d->registry = nullptr;
}

std::shared_ptr<Registry> RegistryFile::getRegistry()
{
    return d->registry;
}

} // namespace io
