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
#ifndef GPUI_SMB_DIRECTORY_H
#define GPUI_SMB_DIRECTORY_H

#include <QString>

#include <memory>

namespace gpui
{

namespace smb
{

class SmbDirectoryPrivate;

class SmbDirectory final
{
public:
    SmbDirectory(const QString& dir);
    ~SmbDirectory() = default;

public:
    bool exists() const;
    bool mkdir(const QString& dir) const;
    bool rmdir(const QString& dir) const;

private:
    static void freePrivate(SmbDirectoryPrivate* p);

    std::unique_ptr<SmbDirectoryPrivate, decltype(&freePrivate)> d;
};

}

}

#endif//GPUI_SMB_DIRECTORY_H
