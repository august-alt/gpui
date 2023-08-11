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

#include "abstractlogger.h"

#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

namespace gpui
{
namespace logger
{
Logger::Logger() {}

void Logger::setLogLevel(int mask)
{
    this->logLevelMask = mask;
}

bool Logger::isLogLevel(int mask)
{
    return this->logLevelMask & mask;
}

void Logger::onDebug(const LoggerMessage &message)
{
    if (isLogLevel(LogLevel::Debug))
    {
        logInfo(message);
    }
}

void Logger::onInfo(const LoggerMessage &message)
{
    if (isLogLevel(LogLevel::Info))
    {
        logInfo(message);
    }
}

void Logger::onWarning(const LoggerMessage &message)
{
    if (isLogLevel(LogLevel::Warning))
    {
        logWarning(message);
    }
}

void Logger::onError(const LoggerMessage &message)
{
    if (isLogLevel(LogLevel::Error))
    {
        logError(message);
    }
}

void Logger::onCritical(const LoggerMessage &message)
{
    if (isLogLevel(LogLevel::Critical))
    {
        logInfo(message);
    }
}
} // namespace logger
} // namespace gpui
