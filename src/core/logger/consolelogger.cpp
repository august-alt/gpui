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

#include "consolelogger.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <unistd.h>

static bool checkColorSupport(int fd)
{
    // TODO(mchernigin): use `tput colors`, and use method bellow only if `tput` returns !0

    bool is_tty      = isatty(fd);
    const char *TERM = std::getenv("TERM");

    return is_tty && TERM != NULL && strcmp(TERM, "dumb") != 0;
}

static std::string colorize(const std::string &text, const char *params)
{
    return std::string("\033[") + params + "m" + text + "\033[0m";
}

namespace gpui
{
namespace logger
{
ConsoleLogger::ConsoleLogger()
{
    this->hasColorSupport = checkColorSupport(STDERR_FILENO);
}

void ConsoleLogger::log(const LoggerMessage &message)
{
    std::string prefix = this->logLevelMap.at(message.msgType);

    if (this->hasColorSupport)
    {
        switch (message.msgType)
        {
        case QtDebugMsg:
            prefix = this->hasColorSupport ? colorize(prefix, "1;96") : prefix;
            break;
        case QtInfoMsg:
            prefix = this->hasColorSupport ? colorize(prefix, "1;34") : prefix;
            break;
        case QtWarningMsg:
            prefix = this->hasColorSupport ? colorize(prefix, "1;33") : prefix;
            break;
        case QtCriticalMsg:
            prefix = this->hasColorSupport ? colorize(prefix, "1;31") : prefix;
            break;
        case QtFatalMsg:
            prefix = this->hasColorSupport ? colorize(prefix, "1;91") : prefix;
            break;
        }
    }

    std::clog << message.getTimeFormatted("%H:%M:%S") << " | " << prefix << ": " << message.message << " ("
              << message.filePath << ":" << message.line << ")" << std::endl;
}
} // namespace logger
} // namespace gpui
