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

#include <QApplication>

namespace gpui
{
class LoggerPrivate;

class GPUI_CORE_EXPORT Logger
{
public:
    explicit Logger();

    static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    Logger(const Logger &) = delete;            // copy ctor
    Logger(Logger &&)      = delete;            // move ctor
    Logger &operator=(const Logger &) = delete; // copy assignment
    Logger &operator=(Logger &&) = delete;      // move assignment

private:
    LoggerPrivate *d = nullptr;
};
} // namespace gpui

#endif // GPUI_LOGGER_H
