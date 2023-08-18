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

namespace gpui
{
namespace logger
{
std::shared_ptr<LoggerManager> LoggerManager::instance = nullptr;

std::shared_ptr<LoggerManager> LoggerManager::getInstance()
{
    if (instance.use_count() == 0)
    {
        instance = std::make_shared<LoggerManager>();
    }

    return instance;
}

void LoggerManager::destroyInstance()
{
    instance.reset();
}

void LoggerManager::addLogger(std::shared_ptr<Logger> logger)
{
    this->loggers.push_back(logger);
}

void LoggerManager::removeLogger(std::shared_ptr<Logger> logger)
{
    const auto search = std::find(this->loggers.begin(), this->loggers.end(), logger);

    if (search != this->loggers.end())
    {
        this->loggers.erase(search);
    }
}

void LoggerManager::clearLoggers()
{
    this->loggers.clear();
}

void LoggerManager::logDebug(const std::string &message,
                             const std::string &file,
                             const std::string &function,
                             const uint32_t line)
{
    for (const auto &logger : this->loggers)
    {
        logger->onDebug(LoggerMessage(message, file, function, line, getCurrentTime(), std::this_thread::get_id()));
    }
}

void LoggerManager::logInfo(const std::string &message,
                            const std::string &file,
                            const std::string &function,
                            const uint32_t line)
{
    for (const auto &logger : this->loggers)
    {
        logger->onInfo(LoggerMessage(message, file, function, line, getCurrentTime(), std::this_thread::get_id()));
    }
}

void LoggerManager::logWarning(const std::string &message,
                               const std::string &file,
                               const std::string &function,
                               const uint32_t line)
{
    for (const auto &logger : this->loggers)
    {
        logger->onWarning(LoggerMessage(message, file, function, line, getCurrentTime(), std::this_thread::get_id()));
    }
}

void LoggerManager::logError(const std::string &message,
                             const std::string &file,
                             const std::string &function,
                             const uint32_t line)
{
    for (const auto &logger : this->loggers)
    {
        logger->onError(LoggerMessage(message, file, function, line, getCurrentTime(), std::this_thread::get_id()));
    }
}

void LoggerManager::logCritical(const std::string &message,
                                const std::string &file,
                                const std::string &function,
                                const uint32_t line)
{
    for (const auto &logger : this->loggers)
    {
        logger->onCritical(LoggerMessage(message, file, function, line, getCurrentTime(), std::this_thread::get_id()));
    }
}

size_t LoggerManager::getLoggerCount() const
{
    return this->loggers.size();
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
