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
#include <iostream>
#include <string>

#include <QApplication>

namespace gpui
{
class LoggerPrivate
{
public:
};

Logger::Logger()
    : d(new LoggerPrivate())
{
    // QLoggingCategory::setFilterRules("*.debug=true\nam.*=true");
    qInstallMessageHandler(messageOutput);
}

void Logger::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    const auto colorize = [](const char *text, const char *params) {
        return std::string("\033[") + params + "m" + text + "\033[0m";
    };

    switch (type)
    {
    case QtDebugMsg:
        std::cerr << colorize("Debug", "1;96");
        break;
    case QtInfoMsg:
        std::cerr << colorize("Info", "1;34");
        break;
    case QtWarningMsg:
        std::cerr << colorize("Warning", "1;33");
        break;
    case QtCriticalMsg:
        std::cerr << colorize("Critical", "1;31");
        break;
    case QtFatalMsg:
        std::cerr << colorize("Fatal", "1;91");
        break;
    }

    std::cerr << ": " << msg.toLocal8Bit().constData();

    if (context.file != nullptr || context.function != nullptr)
    {
        const char *file     = context.file ? context.file : "unknown file";
        const char *function = context.function ? context.function : "unknown function";
        std::cerr << " (" << file << ":" << context.line << ", " << function << ")";
    }
    std::cerr << "\n";
}

} // namespace gpui
