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
/*!
 * \class SyslogLogger syslogger.h
 * \brief Implementation of logger which logs messages to syslog.
 * \ingroup logger
 */

/*!
 * \brief SyslogLogger::SyslogLogger Create new syslog logger.
 *
 * Opens syslog connection.
 */
SyslogLogger::SyslogLogger()
{
    openlog("gpui", (LOG_CONS | LOG_PID), LOG_USER);
}

/*!
 * \brief SyslogLogger::~SyslogLogger Destroy syslog logger.
 *
 * Closes syslog connection.
 */
SyslogLogger::~SyslogLogger()
{
    closelog();
}

/*!
 * \brief SyslogLogger::log Send message to syslog.
 * \param message Message.
 */
void SyslogLogger::log(const LoggerMessage &message)
{
    const char *prefix = this->logLevelMap.at(message.msgType);

    int logFlag = LOG_DEBUG;
    switch (message.msgType)
    {
    case QtInfoMsg:
        logFlag = LOG_INFO;
        break;
    case QtWarningMsg:
        logFlag = LOG_WARNING;
        break;
    case QtCriticalMsg:
        logFlag = LOG_ERR;
        break;
    case QtFatalMsg:
        logFlag = LOG_CRIT;
        break;
    default:
        break;
    }

    syslog(logFlag, "%s: %s (%s:%u)", prefix, message.message.c_str(), message.filePath.c_str(), message.line);
}
} // namespace logger
} // namespace gpui
