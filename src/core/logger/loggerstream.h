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

#ifndef GPUI_LOGGER_STREAM_H
#define GPUI_LOGGER_STREAM_H

#include "loggermanager.h"

#include <memory>
#include <sstream>

namespace gpui
{
namespace logger
{
class GPUI_CORE_EXPORT LoggerStream
{
public:
    LoggerStream(std::shared_ptr<LoggerManager> loggerManager_,
                 int logMask_,
                 const std::string &file,
                 const std::string &function,
                 const uint32_t line);
    ~LoggerStream();

    template<typename T>
    inline LoggerStream &operator<<(const T &value)
    {
        this->buf << value << ' ';
        return (*this);
    }

private:
    LoggerStream(const LoggerStream &) = delete;            // copy ctor
    LoggerStream(LoggerStream &&)      = delete;            // move ctor
    LoggerStream &operator=(const LoggerStream &) = delete; // copy assignment
    LoggerStream &operator=(LoggerStream &&) = delete;      // move assignment

private:
    std::shared_ptr<LoggerManager> loggerManager = nullptr;
    std::stringstream buf                        = {};
    int logMask                                  = 0;
    const std::string &file                      = {};
    const std::string &function                  = {};
    const uint32_t line                          = 0;
};

} // namespace logger
} // namespace gpui
#endif // GPUI_LOGGER_STREAM_H
