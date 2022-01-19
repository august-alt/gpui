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

#include "smbfiletest.h"

#include "../../../../../src/plugins/storage/smb/smbfile.h"

namespace tests
{

using namespace gpui::smb;

void SmbFileTest::remove()
{
    // 1. Remove a file with empty name.

    // 2. Remove a file that does not exist.

    // 3. Remove a file on a write only share.

    // 4. Remove a file on a read only share.

    // 5. Remove a file on a exec only share.
}

void SmbFileTest::rename()
{
    // 1. Rename a file with empty name.

    // 2. Rename a file that does not exist.

    // 3. Rename a file on a write only share.

    // 4. Rename a file on a read only share.

    // 5. Rename a file on a exec only share.
}

void SmbFileTest::link()
{
    // This method is not implemented in a smb file.
}

void SmbFileTest::copy()
{
    // 1. Copy a file with empty name.

    // 2. Copy a file that does not exist.

    // 3. Copy a file on a write only share.

    // 4. Copy a file on a read only share.

    // 5. Copy a file on a exec only share.
}

void SmbFileTest::open()
{
    // 1. Open file with empty file name.

    // 2. Open file on invalid share.

    // 3. Open file on valid share in folder that we have no permission to read, write or execute.

    // 4. Open file with read mode on a write only share.

    // 5. Open file with read mode on a share with read access.

    // 6. Open file with write mode on a read only share.

    // 7. Open file with write mode on exec only share.

    // 8. Open file with write mode on a share with write access.

    // 9. Open file with append mode on a read only share.
}

void SmbFileTest::close()
{
    SmbFile emptyFileName("");
    // 1. Close file with empty file name that does not exists.
    emptyFileName.close();

    // 2. Close file that does not exists.

    // 3. Close file that has not been opened.

    // 4. Close file that has been opened and verify that is has been closed.
}

void SmbFileTest::size()
{
    // 1. Size of a file with empty name.

    // 2. Size of a file which does not exist.

    // 3. Size of a file with 0 size.

    // 4. Size of a file on share we don't have access to.

    // 5. Size of a file we have access to and it has positive size.
}

void SmbFileTest::resize()
{
    // TODO: Write plan.
}

void SmbFileTest::permissions()
{
    // TODO: Write plan.
}

void SmbFileTest::setPermissions()
{
    QFileDevice::Permissions permissions = QFileDevice::ReadOwner | QFileDevice::WriteOther;

    // Negative test cases.
    // 1. Set permissions to file that does not exist.
    SmbFile smbFile("");
    QCOMPARE(smbFile.setPermissions(permissions), false);

    // 2. Get permissions from file that does not exist.
    QCOMPARE(smbFile.permissions(), QFileDevice::Permissions());
}

void SmbFileTest::read()
{
    // TODO: Write plan.
}

void SmbFileTest::write()
{
    // TODO: Write plan.
}

}

QTEST_MAIN(tests::SmbFileTest)
