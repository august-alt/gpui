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

#include "smbsharecreator.h"

#include <QFileInfo>
#include <QProcess>

namespace tests
{

bool SmbShareCreator::createShare(const QString &path, const QString &name, bool allowGuests)
{
    QFileInfo dir(path);

    if (!dir.exists() || !dir.isDir())
    {
        return false;
    }

    if (name.isEmpty())
    {
        return false;
    }

    QString command("net usershare add");

    command += name + ' ' + path + ' ';

    command += QString("guest_ok=%1").arg(allowGuests ? "y" : "n");

    return QProcess::execute(command) == 0;
}

bool SmbShareCreator::removeShare(const QString &name)
{
    if (name.isEmpty())
    {
        return false;
    }

    return QProcess::execute(QString("net usershare delete %1").arg(name)) == 0;
}

}
