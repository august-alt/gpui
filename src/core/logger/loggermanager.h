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

#ifndef GPUI_LOGGER_MANAGER_H
#define GPUI_LOGGER_MANAGER_H

#include "../core.h"
#include "logger.h"

#include <memory>
#include <mutex>
#include <vector>

namespace gpui
{
namespace logger
{
class GPUI_CORE_EXPORT LoggerManager
{
public:
    static std::shared_ptr<LoggerManager> getInstance();
    static void destroyInstance();

    LoggerManager();
    ~LoggerManager() = default;

    void addLogger(std::shared_ptr<Logger> logger);
    void removeLogger(std::shared_ptr<Logger> logger);
    void clearLoggers();
    size_t getLoggerCount() const;

    void logDebug(const std::string &message, const std::string &file, const std::string &function, const uint32_t line);
    void logInfo(const std::string &message, const std::string &file, const std::string &function, const uint32_t line);
    void logWarning(const std::string &message,
                    const std::string &file,
                    const std::string &function,
                    const uint32_t line);
    void logError(const std::string &message, const std::string &file, const std::string &function, const uint32_t line);
    void logCritical(const std::string &message,
                     const std::string &file,
                     const std::string &function,
                     const uint32_t line);

private:
    LoggerManager(const LoggerManager &) = delete;            // copy ctor
    LoggerManager(LoggerManager &&)      = delete;            // move ctor
    LoggerManager &operator=(const LoggerManager &) = delete; // copy assignment
    LoggerManager &operator=(LoggerManager &&) = delete;      // move assignment

private:
    static std::shared_ptr<LoggerManager> instance;
    static std::tm getCurrentTime();

    std::vector<std::shared_ptr<Logger>> loggers = {};

    mutable std::mutex loggerMutex = {};
};
} // namespace logger
} // namespace gpui

#endif // GPUI_LOGGER_MANAGER_H
