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

#ifndef GPUI_COMMAND_GROUP_H
#define GPUI_COMMAND_GROUP_H

#include "../model.h"

#include "command.h"

namespace model {
    namespace command {
        class CommandGroupPrivate;

        class GPUI_MODEL_EXPORT CommandGroup : public AbstractCommand
        {
        public:
            CommandGroup();
            ~CommandGroup();

            void execute() override final;

            size_t addSubCommand(std::unique_ptr<AbstractCommand> subCommand);
            void removeSubCommand(size_t index);
            void replaceCommand(size_t index, std::unique_ptr<AbstractCommand> subCommand);

            bool canExecute() const;
            bool isEmpty() const;

            size_t size() const;

            void clear();

        private:
            CommandGroup(const CommandGroup&)            = delete;   // copy ctor
            CommandGroup(CommandGroup&&)                 = delete;   // move ctor
            CommandGroup& operator=(const CommandGroup&) = delete;   // copy assignment
            CommandGroup& operator=(CommandGroup&&)      = delete;   // move assignment

        private:
            CommandGroupPrivate* d;
        };
    }
}

#endif // GPUI_COMMAND_GROUP_H
