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

#ifndef GPUI_ABSTRACT_LOGGER_H
#define GPUI_ABSTRACT_LOGGER_H

#include "../core.h"
#include "loggermessage.h"

#include <fstream>
#include <unordered_map>
#include <QtMsgHandler>

namespace gpui
{
namespace logger
{
class GPUI_CORE_EXPORT Logger
{
public:
    Logger()          = default;
    virtual ~Logger() = default;

    void setLogLevel(QtMsgType level);
    bool isLogLevel(QtMsgType level);

    void logMessage(const LoggerMessage &message);

private:
    Logger(const Logger &) = delete;            // copy ctor
    Logger(Logger &&)      = delete;            // move ctor
    Logger &operator=(const Logger &) = delete; // copy assignment
    Logger &operator=(Logger &&) = delete;      // move assignment

private:
    virtual void log(const LoggerMessage &message) = 0;

    QtMsgType minLogLevel = QtDebugMsg;

protected:
    const std::unordered_map<QtMsgType, const char *> logLevelMap = {
        {QtDebugMsg, "DEBUG"},
        {QtInfoMsg, "INFO"},
        {QtWarningMsg, "WARNING"},
        {QtCriticalMsg, "CRITICAL"},
        {QtFatalMsg, "FATAL"},
    };
};
} // namespace logger
} // namespace gpui

#endif // GPUI_ABSTRACT_LOGGER_H
