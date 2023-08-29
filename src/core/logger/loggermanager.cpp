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

/*!
 * \brief getCurrentTime Returns current system time.
 * \return Time struct.
 */
static std::tm getCurrentTime()
{
    time_t time;
    std::time(&time);

    // NOTE: localtime returns a pointer to a statically allocated object
    return *std::localtime(&time);
}

namespace gpui
{
namespace logger
{
class LoggerManagerPrivate
{
public:
    std::vector<std::shared_ptr<Logger>> loggers = {};
    mutable std::mutex loggerMutex               = {};
};

/*!
 * \class LoggerManager loggermanager.h
 * \brief Class for managing loggers.
 * \ingroup logger
 *
 * Provides ability to enable new loggers.
 */

std::shared_ptr<LoggerManager> LoggerManager::instance{nullptr};

/*!
 * \brief LoggerManager::LoggerManager Create new LoggerManager.
 *
 * Installs qt message handler to its own static function.
 */
LoggerManager::LoggerManager()
    : d(new LoggerManagerPrivate)
{
    qInstallMessageHandler(LoggerManager::messageHandler);
}

/*!
 * \brief LoggerManager::~LoggerManager Destroy logger manager.
 */
LoggerManager::~LoggerManager()
{
    delete d;
}

/*!
 * \brief LoggerManager::destroyInstance Destroy global logger manager instance.
 */
void LoggerManager::destroyInstance()
{
    instance.reset();
}

/*!
 * \brief LoggerManager::addLogger Add new logger.
 * \param logger New logger.
 *
 * Enables given logger.
 */
void LoggerManager::addLogger(std::shared_ptr<Logger> logger)
{
    std::lock_guard<std::mutex> lockGuardLogger(d->loggerMutex);
    d->loggers.push_back(logger);
}

/*!
 * \brief LoggerManager::removeLogger Remove logger.
 * \param logger Logger.
 *
 * Disables given logger.
 */
void LoggerManager::removeLogger(std::shared_ptr<Logger> logger)
{
    std::lock_guard<std::mutex> lockGuardLogger(d->loggerMutex);
    const auto search = std::find(d->loggers.begin(), d->loggers.end(), logger);

    if (search != d->loggers.end())
    {
        d->loggers.erase(search);
    }
}

/*!
 * \brief LoggerManager::clearLoggers Remove all logger.
 *
 * Disables all loggers.
 */
void LoggerManager::clearLoggers()
{
    d->loggers.clear();
}

/*!
 * \brief LoggerManager::log Log message to all loggers.
 * \param msgType Log level.
 * \param message Message text.
 * \param file File where logger was invoked.
 * \param function Function where logger was invoked.
 * \param line Line where logger was invoked.
 */
void LoggerManager::log(const QtMsgType &msgType,
                        const std::string &message,
                        const std::string &file,
                        const std::string &function,
                        const uint32_t line)
{
    std::lock_guard<std::mutex> lockGuardLogger(d->loggerMutex);
    for (const auto &logger : d->loggers)
    {
        logger->logMessage(
            LoggerMessage(msgType, message, file, function, line, getCurrentTime(), std::this_thread::get_id()));
    }
}

/*!
 * \brief LoggerManager::getLoggerCount Get number of enabled loggers.
 * \return Number of loggers.
 */
size_t LoggerManager::getLoggerCount() const
{
    std::lock_guard<std::mutex> lockGuardLogger(d->loggerMutex);
    return d->loggers.size();
}

/*!
 * \brief LoggerManager::messageHandler Handler of all log messages.
 * \param msgType Log level.
 * \param context Qt context.
 * \param msg Message text.
 */
void LoggerManager::messageHandler(QtMsgType msgType, const QMessageLogContext &context, const QString &msg)
{
    auto logger          = globalInstance();
    std::string file     = context.file ? context.file : "";
    std::string function = context.function ? context.function : "";
    int line             = context.line;

    logger->log(msgType, msg.toStdString(), file, function, line);
}

Q_GLOBAL_STATIC(LoggerManager, loggerInstance)
/*!
 * \brief LoggerManager::globalInstance Get global instance of logger manager.
 * \return Pointer to logger manager.
 */
LoggerManager *LoggerManager::globalInstance()
{
    return loggerInstance();
}
} // namespace logger
} // namespace gpui
