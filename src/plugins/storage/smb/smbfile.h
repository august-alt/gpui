/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd. <org@basealt.ru>
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

#ifndef GPUI_SMB_FILE_H
#define GPUI_SMB_FILE_H

#include <QFile>

#include <memory>

namespace gpui
{
    namespace smb
    {
        class SmbFilePrivate;

        class SmbFile final
        {
        public:
            explicit SmbFile(const QString& name);

            bool remove();
            bool rename(const QString &newName);
            bool link(const QString &newName);
            bool copy(const QString &newName);
            bool open(QFile::OpenMode flags);
            void close();
            qint64 size() const;
            bool resize(qint64 sz);
            QFile::Permissions permissions() const;
            bool setPermissions(const QFile::Permissions& permissionSpec);

            qint64 read(char* buffer, qint64 count);
            qint64 write(const char* buffer, qint64 count);

        private:
            static void freePrivate(SmbFilePrivate* smbFilePrivate);

            int createOpenFlags(QFile::OpenMode flags);
            mode_t createMode(const QFile::Permissions& permissionSpec);
            QFile::Permissions createPermissions(mode_t mode) const;

        private:
            std::unique_ptr<SmbFilePrivate, decltype(&freePrivate)> d;
        };
    }
}

#endif//GPUI_SMB_FILE_H
