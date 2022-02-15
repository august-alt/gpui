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

#include "smbfile.h"

#include "smbclient.h"

#include <QDebug>

namespace gpui
{

namespace smb
{

class SmbFilePrivate
{
public:
    SmbClient client;
    FileHandle handle { nullptr };
    QString fileName { "" };
};

SmbFile::SmbFile(const QString& name)
    : d(new SmbFilePrivate(), &freePrivate)
{
    d->fileName = name;
}

bool SmbFile::remove()
{
    return false;
}

bool SmbFile::rename(const QString &newName)
{
    d->client.closeFile(d->handle);

    int result = d->client.rename(d->fileName, newName);

    if (result < 0)
    {
        qWarning() << "File" << d->fileName << "Error: " << strerror(errno);

        return false;
    }

    d->fileName = newName;

    return true;
}

bool SmbFile::link(const QString &newName)
{
    Q_UNUSED(newName);

    return false;
}

bool SmbFile::copy(const QString &newName)
{
    Q_UNUSED(newName);

    return false;
}

bool SmbFile::open(QFile::OpenMode flags)
{
    bool result = false;
    if (!d->fileName.isEmpty())
    {
        int openFlags = createOpenFlags(flags);
        int creationMode = 0;

        d->handle = d->client.openFile(d->fileName, openFlags, creationMode);

        if (d->handle == nullptr)
        {
            qWarning() << "File" << d->fileName << "Error: " << strerror(errno);
        }

        result = d->handle != 0;
    }

    return result;
}

void SmbFile::close()
{
    if (d->handle != 0)
    {
        d->client.closeFile(d->handle);
    }
}

qint64 SmbFile::size() const
{
    struct stat st;

    memset(&st, 0, sizeof(st));

    bool ok = d->handle != 0 ? d->client.stat(d->handle, st) == 0 : false;
    if (!ok && d->handle == 0)
    {
        ok = d->client.stat(d->fileName, st) == 0;

        qWarning() << "FileName: " << d->fileName << " ok: " << ok;
    }
    if (!ok)
    {
        qWarning() << "File" << d->fileName << "Error: " << strerror(errno);
    }

    if (ok)
    {
        qWarning() << "File: " << d->fileName << "\n"
                   << "st.st_atim"    << st.st_atim.tv_sec  << "\n"
                   << "st.st_blksize" << st.st_blksize      << "\n"
                   << "st.st_blocks"  << st.st_blocks       << "\n"
                   << "st.st_ctim"    << st.st_ctim.tv_sec  << "\n"
                   << "st.st_dev"     << st.st_dev          << "\n"
                   << "st.st_gid"     << st.st_gid          << "\n"
                   << "st.st_ino"     << st.st_ino          << "\n"
                   << "st.st_mode"    << st.st_mode         << "\n"
                   << "st.st_mtim"    << st.st_mtim.tv_sec  << "\n"
                   << "st.st_nlink"   << st.st_nlink        << "\n"
                   << "st.st_rdev"    << st.st_rdev         << "\n"
                   << "st.st_size"    << st.st_size         << "\n"
                   << "st.st_uid"     << st.st_uid          << "\n";
        return static_cast<qint64>(st.st_size);
    }

    return 0;
}

bool SmbFile::resize(qint64 sz)
{
    Q_UNUSED(sz);

    return false;
}

QFileDevice::Permissions SmbFile::permissions() const
{
    struct stat st;

    memset(&st, 0, sizeof(st));

    int result = d->client.stat(d->handle, st);
    if (result < 0)
    {
        qWarning() << "File" << d->fileName << "Error: " << strerror(errno);
    }

    return createPermissions(st.st_mode);
}

bool SmbFile::setPermissions(const QFileDevice::Permissions &permissionSpec)
{
    mode_t permissions = createMode(permissionSpec);
    int result = d->client.setPermissions(d->fileName, permissions);

    if (result < 0)
    {
        qWarning() << "File" << d->fileName << "Error: " << strerror(errno);
    }

    return result >= 0;
}

qint64 SmbFile::read(char* buffer, qint64 count)
{
    qint64 result = d->client.readFile(d->handle, buffer, count);

    if (result < 0)
    {
        qWarning() << "File" << d->fileName << "Error: " << strerror(errno);
    }

    return result;
}

qint64 SmbFile::write(const char *buffer, qint64 count)
{
    qint64 result = d->client.writeFile(d->handle, const_cast<char*>(buffer), count);

    if (result < 0)
    {
        qWarning() << "File" << d->fileName << "Error: " << strerror(errno);
    }

    return result;
}


int SmbFile::createOpenFlags(QIODevice::OpenMode flags)
{
    int result = 0;

    if (flags & QIODevice::ReadOnly)
    {
        if (flags & QIODevice::WriteOnly)
        {
            result = O_RDWR | O_CREAT;
        }
        else
        {
            result = O_RDONLY;
        }
    }
    else if (flags & QIODevice::WriteOnly)
    {
        result = O_WRONLY | O_CREAT;
    }

    if (flags & QIODevice::Append)
    {
        result |= O_APPEND;
    }
    if (flags & QIODevice::Truncate)
    {
        result |= O_TRUNC;
    }

    return result;
}

mode_t SmbFile::createMode(const QFileDevice::Permissions &permissionSpec)
{
    mode_t mode = 0;

    if (permissionSpec & QFileDevice::ReadOwner)
    {
        mode |= S_IRUSR;
    }
    if (permissionSpec & QFileDevice::WriteOwner)
    {
        mode |= S_IWUSR;
    }
    if (permissionSpec & QFileDevice::ExeOwner)
    {
        mode |= S_IXUSR;
    }

    if (permissionSpec & QFileDevice::ReadUser)
    {
        mode |= S_IRUSR;
    }
    if (permissionSpec & QFileDevice::WriteUser)
    {
        mode |= S_IWUSR;
    }
    if (permissionSpec & QFileDevice::ExeUser)
    {
        mode |= S_IXUSR;
    }

    if (permissionSpec & QFileDevice::ReadGroup)
    {
        mode |= S_IRGRP;
    }
    if (permissionSpec & QFileDevice::WriteGroup)
    {
        mode |= S_IWGRP;
    }
    if (permissionSpec & QFileDevice::ExeGroup)
    {
        mode |= S_IXGRP;
    }

    if (permissionSpec & QFileDevice::ReadOther)
    {
        mode |= S_IROTH;
    }
    if (permissionSpec & QFileDevice::WriteOther)
    {
        mode |= S_IWOTH;
    }
    if (permissionSpec & QFileDevice::ExeOther)
    {
        mode |= S_IXOTH;
    }

    return mode;
}

QFileDevice::Permissions SmbFile::createPermissions(mode_t mode) const
{
    QFileDevice::Permissions currentPermissions = 0;

    if (mode & S_IRUSR)
    {
        currentPermissions |= QFileDevice::ReadOwner;
        currentPermissions |= QFileDevice::ReadUser;
    }
    if (mode & S_IWUSR)
    {
        currentPermissions |= QFileDevice::WriteOwner;
        currentPermissions |= QFileDevice::WriteUser;
    }
    if (mode & S_IXUSR)
    {
        currentPermissions |= QFileDevice::ExeOwner;
        currentPermissions |= QFileDevice::ExeUser;
    }

    if (mode & S_IRGRP)
    {
        currentPermissions |= QFileDevice::ReadGroup;
    }
    if (mode & S_IWGRP)
    {
        currentPermissions |= QFileDevice::WriteGroup;
    }
    if (mode & S_IXGRP)
    {
        currentPermissions |= QFileDevice::ExeGroup;
    }

    if (mode & S_IROTH)
    {
        currentPermissions |= QFileDevice::ReadOther;
    }
    if (mode & S_IWOTH)
    {
        currentPermissions |= QFileDevice::WriteOther;
    }
    if (mode & S_IXOTH)
    {
        currentPermissions |= QFileDevice::ExeOther;
    }

    return currentPermissions;
}

void SmbFile::freePrivate(SmbFilePrivate* smbFilePrivate)
{
    delete smbFilePrivate;
}

}

}
