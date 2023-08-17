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

#include "loggerstream.h"

#include <QKeySequence>

namespace gpui
{
namespace logger
{
LoggerStream::LoggerStream(std::shared_ptr<LoggerManager> loggerManager_,
                           int logMask_,
                           const std::string &file_,
                           const std::string &function_,
                           const uint32_t line_)
    : loggerManager(loggerManager_)
    , logMask(logMask_)
    , file(file_)
    , function(function_)
    , line(line_)
{}

LoggerStream::~LoggerStream()
{
    std::string message = this->buf.str();
    if (message.back() == ' ')
    {
        message.pop_back();
    }

    if (this->logMask & LogLevel::Debug)
    {
        this->loggerManager->logDebug(message, this->file, this->function, this->line);
    }
    if (this->logMask & LogLevel::Info)
    {
        this->loggerManager->logInfo(message, this->file, this->function, this->line);
    }
    if (this->logMask & LogLevel::Warning)
    {
        this->loggerManager->logWarning(message, this->file, this->function, this->line);
    }
    if (this->logMask & LogLevel::Error)
    {
        this->loggerManager->logError(message, this->file, this->function, this->line);
    }
    if (this->logMask & LogLevel::Critical)
    {
        this->loggerManager->logCritical(message, this->file, this->function, this->line);
    }
}

inline LoggerStream &LoggerStream::operator<<(const QKeySequence &value) { return *this << value.toString(); }

} // namespace logger
} // namespace gpui
