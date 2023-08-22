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

#ifndef GPUI_SYSLOG_LOGGER_H
#define GPUI_SYSLOG_LOGGER_H

#include "logger.h"
#include "loggermessage.h"
#include "../core.h"

#include <fstream>

namespace gpui
{
namespace logger
{
class GPUI_CORE_EXPORT SyslogLogger : public Logger
{
public:
    SyslogLogger();
    ~SyslogLogger();

private:
    void logDebug(const LoggerMessage &message) override;
    void logInfo(const LoggerMessage &message) override;
    void logWarning(const LoggerMessage &message) override;
    void logCritical(const LoggerMessage &message) override;
    void logFatal(const LoggerMessage &message) override;

    void logMessage(const int log_flag, const std::string &prefix, const LoggerMessage &message);
};
} // namespace logger
} // namespace gpui

#endif // GPUI_SYSLOG_LOGGER_H
