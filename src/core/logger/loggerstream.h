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

#include <QModelIndex>
#include <QString>
#include <QUuid>
#include <QVariant>

namespace gpui
{
namespace logger
{
class GPUI_CORE_EXPORT LoggerStream
{
public:
    LoggerStream(std::shared_ptr<LoggerManager> loggerManager_,
                 int logMask_,
                 const std::string &file_,
                 const std::string &function_,
                 const uint32_t line_);
    ~LoggerStream();

    template<typename T>
    inline LoggerStream &operator<<(const T &value)
    {
        this->buf << value;

        if (this->add_space)
        {
            this->buf << ' ';
        }

        return (*this);
    }

    template<typename T>
    inline LoggerStream &operator<<(const std::vector<T> &value)
    {
        bool old_add_space = this->add_space;
        this->add_space    = false;

        *this << '{';
        for (size_t i = 0; i < value.size(); ++i)
        {
            *this << value[i];
            if (i != value.size() - 1)
            {
                *this << ", ";
            }
        }
        *this << '}';

        this->add_space = old_add_space;

        return *this;
    }

    inline LoggerStream &operator<<(const QList<QString> &value)
    {
        return *this << std::vector<QString>(value.begin(), value.end());
    }

    inline LoggerStream &operator<<(const QStringList &value)
    {
        return *this << std::vector<QString>(value.begin(), value.end());
    }

    inline LoggerStream &operator<<(const QModelIndex &value)
    {
        this->buf << "QModelIndex(" << value.row() << ", " << value.column() << ", " << value.internalPointer() << ", "
                  << value.model() << ')';
        return *this;
    }

    inline LoggerStream &operator<<(const QString &value) { return *this << value.constData(); }
    inline LoggerStream &operator<<(const QVariant &value) { return *this << value.toString(); }
    inline LoggerStream &operator<<(const QUuid &value) { return *this << value.toString(); }
    LoggerStream &operator<<(const QKeySequence &value);

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
    bool add_space                               = true;
};

} // namespace logger
} // namespace gpui
#endif // GPUI_LOGGER_STREAM_H
