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

#include "commandgroup.h"

#include <vector>

namespace model {

namespace command {

class CommandGroupPrivate
{
public:
    std::vector<std::unique_ptr<AbstractCommand>> subCommands;
};

CommandGroup::CommandGroup()
    : d(new CommandGroupPrivate())
{
}

CommandGroup::~CommandGroup()
{
    delete d;
}

void CommandGroup::execute()
{
    for (const auto& command : d->subCommands)
    {
        command->execute();
    }
}

size_t CommandGroup::addSubCommand(std::unique_ptr<AbstractCommand> subCommand)
{
    auto size = d->subCommands.size();

    d->subCommands.emplace_back(std::move(subCommand));

    return size;
}

void CommandGroup::removeSubCommand(size_t index)
{
    if (index < d->subCommands.size())
    {
        d->subCommands.erase(d->subCommands.begin() + index);
    }
}

void CommandGroup::replaceCommand(size_t index, std::unique_ptr<AbstractCommand> subCommand)
{
    if (index < d->subCommands.size())
    {
        d->subCommands[index] = std::move(subCommand);
    }
}

bool CommandGroup::canExecute() const
{
    return !isEmpty();
}

bool CommandGroup::isEmpty() const
{
    return d->subCommands.empty();
}

size_t CommandGroup::size() const
{
    return d->subCommands.size();
}

void CommandGroup::clear()
{
    d->subCommands.clear();
}

}

}
