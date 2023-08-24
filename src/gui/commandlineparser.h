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

#ifndef GPUI_COMMAND_LINE_PARSER_H
#define GPUI_COMMAND_LINE_PARSER_H

#include "gui.h"

#include "../gui/commandlineoptions.h"
#include "../core/logger/prelude.h"

#include <QApplication>
#include <QCommandLineParser>

namespace gpui {
    class CommandLineParserPrivate;

    class GPUI_GUI_EXPORT CommandLineParser
    {
    public:
        enum CommandLineParseResult
        {
            CommandLineOk,
            CommandLineError,
            CommandLineVersionRequested,
            CommandLineHelpRequested
        };

    public:
        CommandLineParser(QApplication& application);
        ~CommandLineParser();

        CommandLineParseResult parseCommandLine(CommandLineOptions *options, QString *errorMessage);

        void showHelp() const;
        void showVersion() const;

    private:
        CommandLineParser(const CommandLineParser&)            = delete;   // copy ctor
        CommandLineParser(CommandLineParser&&)                 = delete;   // move ctor
        CommandLineParser& operator=(const CommandLineParser&) = delete;   // copy assignment
        CommandLineParser& operator=(CommandLineParser&&)      = delete;   // move assignment

    private:
        CommandLineParserPrivate* d;
    };
}

#endif // GPUI_COMMAND_LINE_PARSER_H
