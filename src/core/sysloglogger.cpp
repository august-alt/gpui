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

#include "sysloglogger.h"

#include <sstream>
#include <syslog.h>

namespace gpui
{
namespace logger
{
SyslogLogger::SyslogLogger()
{
    openlog("gpui-main", (LOG_CONS | LOG_PERROR | LOG_PID), LOG_DAEMON);
}

SyslogLogger::~SyslogLogger()
{
    closelog();
}

void SyslogLogger::logDebug(const LoggerMessage &message)
{
    logMessage(LOG_DEBUG, "DEBUG", message);
}

void SyslogLogger::logInfo(const LoggerMessage &message)
{
    logMessage(LOG_INFO, "INFO", message);
}

void SyslogLogger::logWarning(const LoggerMessage &message)
{
    logMessage(LOG_WARNING, "WARNING", message);
}

void SyslogLogger::logError(const LoggerMessage &message)
{
    logMessage(LOG_ERR, "ERROR", message);
}

void SyslogLogger::logCritical(const LoggerMessage &message)
{
    logMessage(LOG_CRIT, "CRITICAL", message);
}

void SyslogLogger::logMessage(const int log_flag, const std::string &prefix, const LoggerMessage &message)
{
    syslog(log_flag, "%s: %s (%s:%u)", prefix.c_str(), message.message.c_str(), message.filePath.c_str(), message.line);
}
} // namespace logger
} // namespace gpui
