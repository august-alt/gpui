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

#include "core.h"
#include "loggermessage.h"

#include <fstream>
#include <QApplication>
#include <qdir.h>

namespace gpui
{
namespace logger
{
enum LogLevel
{
    Debug    = 1 << 0,
    Info     = 1 << 1,
    Warning  = 1 << 2,
    Error    = 1 << 3,
    Critical = 1 << 4,
};

class GPUI_CORE_EXPORT Logger
{
public:
public:
    Logger();
    virtual ~Logger() = default;
    void setLogLevel(int mask);
    bool isLogLevel(int mask);

    void onDebug(const LoggerMessage &message);
    void onInfo(const LoggerMessage &message);
    void onWarning(const LoggerMessage &message);
    void onError(const LoggerMessage &message);
    void onCritical(const LoggerMessage &message);

private:
    Logger(const Logger &) = delete;            // copy ctor
    Logger(Logger &&)      = delete;            // move ctor
    Logger &operator=(const Logger &) = delete; // copy assignment
    Logger &operator=(Logger &&) = delete;      // move assignment

private:
    virtual void logDebug(const LoggerMessage &message)    = 0;
    virtual void logInfo(const LoggerMessage &message)     = 0;
    virtual void logWarning(const LoggerMessage &message)  = 0;
    virtual void logError(const LoggerMessage &message)    = 0;
    virtual void logCritical(const LoggerMessage &message) = 0;

    int logLevelMask = 0;
};
} // namespace logger
} // namespace gpui

#endif // GPUI_ABSTRACT_LOGGER_H
