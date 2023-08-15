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

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <QApplication>
#include <qdir.h>

namespace gpui
{
namespace logger
{
ConsoleLogger::ConsoleLogger() {}

void ConsoleLogger::logDebug(const LoggerMessage &message)
{
    std::string prefix = "DEBUG";
    prefix = checkColorSupport(STDERR_FILENO) ? colorize(prefix, "1;96") : prefix;
    logMessage(prefix, message);
}

void ConsoleLogger::logInfo(const LoggerMessage &message)
{
    std::string prefix = "INFO";
    prefix = checkColorSupport(STDERR_FILENO) ? colorize(prefix, "1;34") : prefix;
    logMessage(prefix, message);
}

void ConsoleLogger::logWarning(const LoggerMessage &message)
{
    std::string prefix = "WARNING";
    prefix = checkColorSupport(STDERR_FILENO) ? colorize(prefix, "1;33") : prefix;
    logMessage(prefix, message);
}

void ConsoleLogger::logError(const LoggerMessage &message)
{
    std::string prefix = "ERROR";
    prefix = checkColorSupport(STDERR_FILENO) ? colorize(prefix, "1;31") : prefix;
    logMessage(prefix, message);
}

void ConsoleLogger::logCritical(const LoggerMessage &message)
{
    std::string prefix = "CRITICAL";
    prefix = checkColorSupport(STDERR_FILENO) ? colorize(prefix, "1;91") : prefix;
    logMessage(prefix, message);
}

void ConsoleLogger::logMessage(const std::string &prefix, const LoggerMessage &message)
{
    std::cerr << prefix << ": " << message.message << " (" << message.filePath << ":" << message.line << ")";
}

bool ConsoleLogger::checkColorSupport(int fd)
{
    // TODO(mchernigin): use `tput color`, and use method bellow only if `tput` returns !0

    bool is_tty      = isatty(fd);
    const char *TERM = std::getenv("TERM");

    return is_tty && TERM != NULL && strcmp(TERM, "dumb") != 0;
}

std::string ConsoleLogger::colorize(const std::string &text, const char *params)
{
    return std::string("\033[") + params + "m" + text + "\033[0m";
}
} // namespace logger
} // namespace gpui
