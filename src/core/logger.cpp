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
#include <memory>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

#include <QApplication>
#include <QFileInfo>
#include <qdebug.h>
#include <qdir.h>

namespace gpui
{
typedef void(QtMessageHandler)(QtMsgType, const QMessageLogContext &, const QString &);
static std::function<QtMessageHandler> bindVersion;
static void messageOutputStatic(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    bindVersion(type, context, msg);
}

Logger::Logger(const char *app_name, uint8_t ol)
    : APP_NAME(app_name)
    , output_locations(ol)
{
    using namespace std::placeholders;

    bindVersion = std::bind(Logger::outputMessage, this, _1, _2, _3);
    qInstallMessageHandler(messageOutputStatic);

    if (this->output_locations & Output::StdErr)
    {
        this->stdErrSupportsColor = checkColorSupport(STDERR_FILENO);
    }
    if (this->output_locations & Output::Syslog)
    {
        openlog(this->APP_NAME, (LOG_CONS | LOG_PERROR | LOG_PID), LOG_DAEMON);
    }
    if (this->output_locations & Output::File)
    {
        auto logDirPath = QDir::home().path() + "/.local/share/gpui";
        if (!QDir(logDirPath).exists())
        {
            QDir().mkpath(logDirPath);
        }

        std::string logFile = logDirPath.toStdString() + "/gpui.log";
        this->logFileStream.open(logFile, std::fstream::out | std::fstream::app);
    }
}

Logger::~Logger()
{
    if (this->output_locations & Output::Syslog)
    {
        closelog();
    }
    if (this->output_locations & Output::File)
    {
        this->logFileStream.close();
    }
}

void Logger::outputMessage(Logger *logger, QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    std::stringstream buf;

    buf << ": " << msg.toLocal8Bit().constData();
    if (context.file != nullptr || context.function != nullptr)
    {
        const char *file     = context.file ? context.file : "unknown file";
        const char *function = context.function ? context.function : "unknown function";
        buf << " (" << file << ":" << context.line << ", " << function << ")";
    }
    buf << "\n";

    std::string messageWithoutPrefix = buf.str();

    if (logger->output_locations & Output::StdErr)
    {
        std::cerr << logger->getPrefix(type, Output::StdErr) << messageWithoutPrefix;
    }
    if (logger->output_locations & Output::Syslog)
    {
        const std::string message = logger->getPrefix(type, Output::StdErr) + messageWithoutPrefix;
        logger->outputMessageToSyslog(type, message.c_str());
    }
    if (logger->output_locations & Output::File)
    {
        logger->logFileStream << logger->getPrefix(type, Output::File) << messageWithoutPrefix;
    }
}

std::string Logger::getPrefix(QtMsgType type, Output output) const
{
    bool shouldColorize = output == Output::StdErr && this->stdErrSupportsColor;

    auto tryColorize = [&shouldColorize](const char *text, const char *params) {
        if (!shouldColorize)
        {
            return std::string(text);
        }
        return std::string("\033[") + params + "m" + text + "\033[0m";
    };

    switch (type)
    {
    case QtDebugMsg:
        return tryColorize("Debug", "1;96");
    case QtInfoMsg:
        return tryColorize("Info", "1;34");
    case QtWarningMsg:
        return tryColorize("Warning", "1;33");
    case QtCriticalMsg:
        return tryColorize("Critical", "1;31");
    case QtFatalMsg:
        return tryColorize("Fatal", "1;91");
    }

    // NOTE(mchernigin): Should be unreachable
    return "";
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

bool Logger::checkColorSupport(int fd)
{
    // TODO(mchernigin): use `tput color`, and use method bellow only if `tput` returns !0

    bool is_tty      = isatty(fd);
    const char *TERM = std::getenv("TERM");

    return is_tty && TERM != NULL && strcmp(TERM, "dumb") != 0;
}

} // namespace gpui
