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

#include "loggermanager.h"

#include <algorithm>
#include <iostream>

namespace gpui
{
namespace logger
{
class LoggerManagerPrivate
{
public:
    std::vector<std::shared_ptr<Logger>> loggers = {};
    mutable std::mutex loggerMutex = {};
};

std::shared_ptr<LoggerManager> LoggerManager::instance{nullptr};

LoggerManager::LoggerManager()
    : d(new LoggerManagerPrivate)
{
    qInstallMessageHandler(LoggerManager::messageHandler);
}

LoggerManager::~LoggerManager()
{
    delete d;
}

void LoggerManager::destroyInstance()
{
    instance.reset();
}

void LoggerManager::addLogger(std::shared_ptr<Logger> logger)
{
    std::lock_guard<std::mutex> lockGuardLogger(d->loggerMutex);
    d->loggers.push_back(logger);
}

void LoggerManager::removeLogger(std::shared_ptr<Logger> logger)
{
    std::lock_guard<std::mutex> lockGuardLogger(d->loggerMutex);
    const auto search = std::find(d->loggers.begin(), d->loggers.end(), logger);

    if (search != d->loggers.end())
    {
        d->loggers.erase(search);
    }
}

void LoggerManager::clearLoggers()
{
    d->loggers.clear();
}

void LoggerManager::logDebug(const std::string &message,
                             const std::string &file,
                             const std::string &function,
                             const uint32_t line)
{
    std::lock_guard<std::mutex> lockGuardLogger(d->loggerMutex);
    for (const auto &logger : d->loggers)
    {
        logger->onDebug(LoggerMessage(message, file, function, line, getCurrentTime(), std::this_thread::get_id()));
    }
}

void LoggerManager::logInfo(const std::string &message,
                            const std::string &file,
                            const std::string &function,
                            const uint32_t line)
{
    std::lock_guard<std::mutex> lockGuardLogger(d->loggerMutex);
    for (const auto &logger : d->loggers)
    {
        logger->onInfo(LoggerMessage(message, file, function, line, getCurrentTime(), std::this_thread::get_id()));
    }
}

void LoggerManager::logWarning(const std::string &message,
                               const std::string &file,
                               const std::string &function,
                               const uint32_t line)
{
    std::lock_guard<std::mutex> lockGuardLogger(d->loggerMutex);
    for (const auto &logger : d->loggers)
    {
        logger->onWarning(LoggerMessage(message, file, function, line, getCurrentTime(), std::this_thread::get_id()));
    }
}

void LoggerManager::logError(const std::string &message,
                             const std::string &file,
                             const std::string &function,
                             const uint32_t line)
{
    std::lock_guard<std::mutex> lockGuardLogger(d->loggerMutex);
    for (const auto &logger : d->loggers)
    {
        logger->onError(LoggerMessage(message, file, function, line, getCurrentTime(), std::this_thread::get_id()));
    }
}

void LoggerManager::logCritical(const std::string &message,
                                const std::string &file,
                                const std::string &function,
                                const uint32_t line)
{
    std::lock_guard<std::mutex> lockGuardLogger(d->loggerMutex);
    for (const auto &logger : d->loggers)
    {
        logger->onCritical(LoggerMessage(message, file, function, line, getCurrentTime(), std::this_thread::get_id()));
    }
}

size_t LoggerManager::getLoggerCount() const
{
    std::lock_guard<std::mutex> lockGuardLogger(d->loggerMutex);
    return d->loggers.size();
}

void LoggerManager::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    auto logger = globalInstance();
    std::string file = context.file ? context.file : "";
    std::string function = context.function ? context.function : "";
    int line = context.line;

    std::cerr << type << std::endl;
    switch (type)
    {
        case QtDebugMsg:
            logger->logDebug(msg.toStdString(), file, function, line);
            break;
        case QtInfoMsg:
            logger->logInfo(msg.toStdString(), file, function, line);
            break;
        case QtWarningMsg:
            logger->logWarning(msg.toStdString(), file, function, line);
            break;
        case QtCriticalMsg:
            logger->logError(msg.toStdString(), file, function, line);
            break;
        case QtFatalMsg:
            logger->logCritical(msg.toStdString(), file, function, line);
            break;
    }
}

Q_GLOBAL_STATIC(LoggerManager, loggerInstance)
LoggerManager *LoggerManager::globalInstance()
{
    return loggerInstance();
}

std::tm LoggerManager::getCurrentTime()
{
    time_t time;
    std::time(&time);

    // NOTE: localtime returns a pointer to a statically allocated object
    return *std::localtime(&time);
}

} // namespace logger
} // namespace gpui
