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

#include "command.h"

namespace model
{

namespace command
{

class LambdaCommandPrivate
{
public:
    std::function<void ()> function;

    LambdaCommandPrivate(std::function<void ()> newFunction)
        : function(newFunction)
    {}

private:
    LambdaCommandPrivate(const LambdaCommandPrivate&)            = delete;   // copy ctor
    LambdaCommandPrivate(LambdaCommandPrivate&&)                 = delete;   // move ctor
    LambdaCommandPrivate& operator=(const LambdaCommandPrivate&) = delete;   // copy assignment
    LambdaCommandPrivate& operator=(LambdaCommandPrivate&&)      = delete;   // move assignment

};

LambdaCommand::LambdaCommand(std::function<void ()> function)
    : d(new LambdaCommandPrivate(function))
{
}

LambdaCommand::~LambdaCommand()
{
    delete d;
}

void LambdaCommand::execute()
{
    d->function();
}

}

}
