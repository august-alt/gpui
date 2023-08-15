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

#include "logger.h"

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <syslog.h>

#include <QApplication>
#include <QFileInfo>

namespace gpui
{
typedef void(QtMessageHandler)(QtMsgType, const QMessageLogContext &, const QString &);
static std::function<QtMessageHandler> m;
static void messageOutputStatic(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    m(type, context, msg);
}

Logger::Logger(const char *app_name, uint8_t ol)
    : app_name(app_name)
    , output_locations(ol)
{
    using namespace std::placeholders;

    m = std::bind(Logger::outputMessage, this, _1, _2, _3);
    qInstallMessageHandler(messageOutputStatic);

    if (this->output_locations & Output::Syslog)
    {
        openlog(this->app_name, (LOG_CONS | LOG_PERROR | LOG_PID), LOG_DAEMON);
    }
}

Logger::~Logger()
{
    if (this->output_locations & Output::Syslog)
    {
        closelog();
    }
}

void Logger::outputMessage(const Logger *logger, QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    std::stringstream buf;

    const auto colorize = [](const char *text, const char *params) {
        return std::string(text);
        return std::string("\033[") + params + "m" + text + "\033[0m";
    };

    switch (type)
    {
    case QtDebugMsg:
        buf << colorize("Debug", "1;96");
        break;
    case QtInfoMsg:
        buf << colorize("Info", "1;34");
        break;
    case QtWarningMsg:
        buf << colorize("Warning", "1;33");
        break;
    case QtCriticalMsg:
        buf << colorize("Critical", "1;31");
        break;
    case QtFatalMsg:
        buf << colorize("Fatal", "1;91");
        break;
    }

    buf << ": " << msg.toLocal8Bit().constData();

    if (context.file != nullptr || context.function != nullptr)
    {
        const char *file     = context.file ? context.file : "unknown file";
        const char *function = context.function ? context.function : "unknown function";
        buf << " (" << file << ":" << context.line << ", " << function << ")";
    }
    buf << "\n";

    std::string message = buf.str();

    if (logger->output_locations & Output::Console)
    {
        std::cerr << message;
    }
    if (logger->output_locations & Output::Syslog)
    {
        logger->outputMessageToSyslog(type, message.c_str());
    }
    if (logger->output_locations & Output::File)
    {
        // TODO(mchernigin): not implemented
    }
}

void Logger::outputMessageToSyslog(QtMsgType type, const char *message) const
{
    int log_flag;
    switch (type)
    {
    case QtDebugMsg:
        log_flag = LOG_DEBUG;
        break;
    case QtInfoMsg:
        log_flag = LOG_INFO;
        break;
    case QtWarningMsg:
        log_flag = LOG_WARNING;
        break;
    case QtCriticalMsg:
        log_flag = LOG_CRIT;
        break;
    case QtFatalMsg:
        log_flag = LOG_ERR;
        break;
    }
    syslog(log_flag, "%s", message);
}

} // namespace gpui
