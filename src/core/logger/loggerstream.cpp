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
LoggerStream::LoggerStream(std::shared_ptr<LoggerManager> loggerManager,
                           int logMask,
                           const std::string &file,
                           const std::string &function,
                           const uint32_t line)
    : d(new LoggerStreamPrivate())
{
    d->loggerManager = loggerManager;
    d->logMask       = logMask;
    d->file          = file;
    d->function      = function;
    d->line          = line;
}

LoggerStream::~LoggerStream()
{
    std::string message = d->buf.str();
    if (message.back() == ' ')
    {
        message.pop_back();
    }

    if (d->logMask & LogLevel::Debug)
    {
        d->loggerManager->logDebug(message, d->file, d->function, d->line);
    }
    if (d->logMask & LogLevel::Info)
    {
        d->loggerManager->logInfo(message, d->file, d->function, d->line);
    }
    if (d->logMask & LogLevel::Warning)
    {
        d->loggerManager->logWarning(message, d->file, d->function, d->line);
    }
    if (d->logMask & LogLevel::Error)
    {
        d->loggerManager->logError(message, d->file, d->function, d->line);
    }
    if (d->logMask & LogLevel::Critical)
    {
        d->loggerManager->logCritical(message, d->file, d->function, d->line);
    }

    delete d;
}

LoggerStream &LoggerStream::operator<<(const QKeySequence &value)
{
    return *this << value.toString();
}

} // namespace logger
} // namespace gpui
