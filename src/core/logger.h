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

#ifndef GPUI_LOGGER_H
#define GPUI_LOGGER_H

#include "core.h"

#include <fstream>
#include <QApplication>
#include <qdir.h>

namespace gpui
{
class GPUI_CORE_EXPORT Logger
{
public:
    enum Output
    {
        StdErr = 1 << 0,
        Syslog = 1 << 1,
        File   = 1 << 2,
    };

public:
    explicit Logger(const char *app_name, uint8_t output_locations = 0);
    ~Logger();

    static void outputMessage(Logger *logger, QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    Logger(const Logger &) = delete;            // copy ctor
    Logger(Logger &&)      = delete;            // move ctor
    Logger &operator=(const Logger &) = delete; // copy assignment
    Logger &operator=(Logger &&) = delete;      // move assignment

private:
    const char *APP_NAME        = "";
    bool stdErrSupportsColor    = false;
    uint8_t output_locations    = {};
    std::ofstream logFileStream = {};

private:
    void outputMessageToSyslog(QtMsgType type, const char *message) const;
    std::string getPrefix(QtMsgType type, Output out) const;
    static bool checkColorSupport(int fd);
};
} // namespace gpui

#endif // GPUI_LOGGER_H
