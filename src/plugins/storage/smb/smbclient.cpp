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

#include "smbclient.h"

namespace gpui
{

namespace smb
{

const int SMB_FREE_EVEN_IF_BUSY = 1;
const int SMB_DEBUG_LEVEL = 5;

namespace
{

QByteArray defaultUserName("guest");
QByteArray defaultPassword("");
QByteArray defaultWorkGroup("WORKGROUP");

}

SmbClient::SmbClient()
    : SmbClient(defaultUserName, defaultPassword)
{
}

SmbClient::SmbClient(const QString &userName, const QString &password)
    : ctx(createContext(userName, password, &authenticationCallBack), freeContext)
{
}

SmbClient::SmbClient(const QString &userName, const QString &password, const QString &workGroup)
    : ctx(createContext(userName, password, workGroup, &authenticationCallBack), freeContext)
{
}

ContextPtr SmbClient::createContext(AuthenticationFunction function)
{
    if (!function)
    {
        function = &authenticationCallBack;
    }

    ContextPtr newContext = smbc_new_context();

    if (newContext)
    {
        smbc_setDebug(newContext, SMB_DEBUG_LEVEL);
        smbc_setOptionUseKerberos(newContext, true);
        smbc_setOptionFallbackAfterKerberos(newContext, true);
        smbc_setFunctionAuthData(newContext, function);
        smbc_setOptionCaseSensitive(newContext, false);

        if (smbc_init_context(newContext) == nullptr)
        {
            smbc_free_context(newContext, SMB_FREE_EVEN_IF_BUSY);
            newContext = nullptr;
        }
    }

    return newContext;
}

ContextPtr SmbClient::createContext(const QString &currentUserName, const QString &currentPassword,
                                    AuthenticationFunction function)
{
    defaultUserName = currentUserName.toLocal8Bit();
    defaultPassword = currentPassword.toLocal8Bit();

    return createContext(function);
}

ContextPtr SmbClient::createContext(const QString &currentUserName, const QString &currentPassword,
                                    const QString& currentWorkGroup, AuthenticationFunction function)
{
    defaultWorkGroup = currentWorkGroup.toLocal8Bit();

    return createContext(currentUserName, currentPassword, function);
}

void SmbClient::freeContext(ContextPtr context)
{
    if (context)
    {
        smbc_free_context(context, SMB_FREE_EVEN_IF_BUSY);
    }
}

FileHandle SmbClient::openFile(const QString &fileName, int flags, mode_t mode)
{
    return smbc_getFunctionOpen(ctx.get())(ctx.get(), fileName.toLocal8Bit().constData(), flags, mode);
}

FileHandle SmbClient::openFileReadOnly(const QString &fileName)
{
    return openFile(fileName);
}

ssize_t SmbClient::readFile(FileHandle handle, void *buffer, size_t count)
{
    return smbc_getFunctionRead(ctx.get())(ctx.get(), handle, buffer, count);
}

ssize_t SmbClient::writeFile(FileHandle handle, void *buffer, size_t count)
{
    return smbc_getFunctionWrite(ctx.get())(ctx.get(), handle, buffer, count);
}

off_t SmbClient::seekFile(FileHandle handle, off_t offset, int whence)
{
    return smbc_getFunctionLseek(ctx.get())(ctx.get(), handle, offset, whence);
}

int SmbClient::rename(const QString& oldName, const QString &newName)
{
    std::unique_ptr<Context, decltype (&freeContext)> context(createContext(&authenticationCallBack),
                                                              freeContext);
    int result = smbc_getFunctionRename(ctx.get())
                                       (ctx.get(),
                                        oldName.toLocal8Bit().constData(),
                                        context.get(),
                                        newName.toLocal8Bit().constData()
                                       );
    return result;
}

int SmbClient::stat(const QString &path, struct stat& st)
{
    return smbc_getFunctionStat(ctx.get())(ctx.get(), path.toLocal8Bit().constData(), &st);
}

int SmbClient::stat(FileHandle handle, struct stat& st)
{
    return smbc_getFunctionFstat(ctx.get())(ctx.get(), handle, &st);
}

FileHandle SmbClient::openDir(const QString &directoryName)
{
    return smbc_getFunctionOpendir(ctx.get())(ctx.get(), directoryName.toLocal8Bit().constData());
}

int SmbClient::createDir(const QString &directoryName, mode_t mode)
{
    return smbc_getFunctionMkdir(ctx.get())(ctx.get(), directoryName.toLocal8Bit().constData(), mode);
}

int SmbClient::deleteDir(const QString &directoryName)
{
    return smbc_getFunctionRmdir(ctx.get())(ctx.get(), directoryName.toLocal8Bit().constData());
}

bool SmbClient::setPermissions(const QString &path, mode_t mode)
{    
    return smbc_getFunctionChmod(ctx.get())(ctx.get(), path.toLocal8Bit().constData(), mode);
}

void SmbClient::authenticationCallBack(const char *server, const char *share, char *currentWorkGroup,
                                       int workGroupLength, char *currentUser, int userLength,
                                       char *currentPassword, int passwordLength)
{
    Q_UNUSED(server);
    Q_UNUSED(share);

    strncpy(currentWorkGroup, defaultWorkGroup.constData(), workGroupLength - 1);
    strncpy(currentUser, defaultUserName.constData(), userLength - 1);
    strncpy(currentPassword, defaultPassword.constData(), passwordLength - 1);
}

void SmbClient::closeFile(FileHandle handle)
{
    smbc_getFunctionClose(ctx.get())(ctx.get(), handle);
}

}

}
