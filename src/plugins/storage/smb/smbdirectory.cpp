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

#include "smbdirectory.h"

#include "smbclient.h"

#include <QDebug>

namespace gpui
{

namespace smb
{

class SmbDirectoryPrivate
{
public:
    SmbClient client;
    QString path;

    SmbDirectoryPrivate()
        : client()
        , path("")
    {}
};

SmbDirectory::SmbDirectory(const QString& dir)
    : d(new SmbDirectoryPrivate(), &freePrivate)
{
    d->path = dir;
}

bool SmbDirectory::exists() const
{
    bool result = false;

    if (!d->path.isEmpty())
    {
        struct stat st;

        memset(&st, 0, sizeof(st));

        if (d->client.stat(d->path, st) >= 0)
        {
            return true;
        }

        qWarning() << "Directory: " << d->path << "Error: " << strerror(errno);
    }

    return result;
}

bool SmbDirectory::mkdir(const QString &dir) const
{
    int operationResult = d->client.createDir(dir, S_IRWXU | S_IRWXG | S_IRWXO);

    if (operationResult < 0)
    {
        qWarning() << "Directory: " << dir << "Error: " << strerror(errno);

        return false;
    }

    d->path = dir;

    return true;
}

bool SmbDirectory::rmdir(const QString &dir) const
{
    int operationResult = d->client.deleteDir(dir);

    if (operationResult < 0)
    {
        qWarning() << "Directory: " << dir << "Error: " << strerror(errno);

        return false;
    }

    d->path.clear();

    return true;
}

void SmbDirectory::freePrivate(SmbDirectoryPrivate *p)
{
    delete p;
}

}

}
