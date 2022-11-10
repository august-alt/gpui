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

#ifndef GPUI_COMMAND_H
#define GPUI_COMMAND_H

#include <functional>
#include <memory>

namespace model
{
namespace command
{
class AbstractCommand
{
public:
    virtual ~AbstractCommand() = default;

    virtual void execute() = 0;

protected:
    AbstractCommand() = default;
};

class LambdaCommandPrivate;

class LambdaCommand : public AbstractCommand
{
public:
    LambdaCommand(std::function<void()> function);
    ~LambdaCommand();

    void execute() override final;

private:
    LambdaCommand(const LambdaCommand &) = delete;            // copy ctor
    LambdaCommand(LambdaCommand &&)      = delete;            // move ctor
    LambdaCommand &operator=(const LambdaCommand &) = delete; // copy assignment
    LambdaCommand &operator=(LambdaCommand &&) = delete;      // move assignment

private:
    LambdaCommandPrivate *d;
};
} // namespace command
} // namespace model

#endif // GPUI_COMMAND_H
