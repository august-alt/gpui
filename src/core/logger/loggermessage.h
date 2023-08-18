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

#ifndef GPUI_LOGGER_MESSAGE_H
#define GPUI_LOGGER_MESSAGE_H

#include <ctime>
#include <string>
#include <thread>

namespace gpui
{
namespace logger
{

class LoggerMessage
{
public:
    LoggerMessage(const std::string &message_,
                  const std::string &filePath_,
                  const std::string &functionName_,
                  const uint32_t line_,
                  const std::tm &time_,
                  const std::thread::id &threadId_)
        : message(message_)
        , filePath(filePath_)
        , functionName(functionName_)
        , line(line_)
        , time(time_)
        , threadId(threadId_)
    {}

    // __attribute__((__format__ (__strftime__, 2, 0)))
    std::string getTimeFormatted(const char *format) const
    {
        char timeString[50];
        // NOTE(mchernigin): is it even a good idea? if it is, do compilers actually support this?
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
        strftime(timeString, 50, format, &time);
#pragma GCC diagnostic pop
        return std::string(timeString);
    }

    const std::string message;
    const std::string filePath;
    const std::string functionName;
    const uint32_t line;
    const std::tm time;
    const std::thread::id threadId;
};
} // namespace logger
} // namespace gpui

#endif // GPUI_LOGGER_MESSAGE_H
