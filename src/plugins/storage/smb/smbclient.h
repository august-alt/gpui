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

#ifndef GPUI_SMB_CLIENT_H
#define GPUI_SMB_CLIENT_H

#include <memory>

#include <QUrl>

#include <libsmbclient.h>

namespace gpui
{
    namespace smb {

        typedef SMBCCTX   Context;
        typedef Context*  ContextPtr;
        typedef SMBCFILE* FileHandle;

        typedef smbc_get_auth_data_fn AuthenticationFunction;

        /*!
         * \brief The SmbClient class wraps libsmbclient calls, provides handling of creation and deletion of samba
         * context.
         */
        class SmbClient final
        {
        public:
            SmbClient();

            /*!
             * \brief SmbClient
             * \param userName      Username of user.
             * \param password      Password of user.
             */
            SmbClient(const QString& userName, const QString& password);

            /*!
             * \brief SmbClient
             * \param userName      Username of user.
             * \param password      Password of user.
             * \param workGroup     Work group to use.
             */
            SmbClient(const QString& userName, const QString& password, const QString& workGroup);

            /*!
             * \ingroup             file
             * \brief openFile      Open a file on an SMB server.
             *
             * \param fileName      The smb url of the file to be opened.
             *
             * \param flags         Is one of O_RDONLY, O_WRONLY or O_RDWR which
             *                      request opening  the  file  read-only,write-only
             *                      or read/write. flags may also be bitwise-or'd with
             *                      one or  more of  the following:
             *                      O_CREAT - If the file does not exist it will be
             *                      created.
             *                      O_EXCL - When  used with O_CREAT, if the file
             *                      already exists it is an error and the open will
             *                      fail.
             *                      O_TRUNC - If the file already exists it will be
             *                      truncated.
             *                      O_APPEND The  file  is  opened  in  append mode
             *
             * \param mode          mode specifies the permissions to use if a new
             *                      file is created.  It  is  modified  by  the
             *                      process's umask in the usual way: the permissions
             *                      of the created file are (mode & ~umask)
             *
             *                      Not currently use, but there for future use.
             *                      We will map this to SYSTEM, HIDDEN, etc bits
             *                      that reverses the mapping that smbc_fstat does.
             *
             * \return              Valid file handle, < 0 on error with errno set:
             *                      - ENOMEM  Out of memory
             *                      - EINVAL if an invalid parameter passed, like no
             *                        file, or smbc_init not called.
             *                      - EEXIST  pathname already exists and O_CREAT and
             *                        O_EXCL were used.
             *                      - EISDIR  pathname  refers  to  a  directory  and
             *                        the access requested involved writing.
             *                      - EACCES  The requested access to the file is not
             *                        allowed
             *                      - ENODEV The requested share does not exist
             *                      - ENOTDIR A file on the path is not a directory
             *                      - ENOENT  A directory component in pathname does
             *                        not exist.
             *
             * \note                This call uses an underlying routine that may create
             *                      a new connection to the server specified in the URL.
             *                      If the credentials supplied in the URL, or via the
             *                      auth_fn in the smbc_init call, fail, this call will
             *                      try again with an empty username and password. This
             *                      often gets mapped to the guest account on some machines.
             */
            FileHandle openFile(const QString& fileName, int flags = O_RDONLY, mode_t mode = 0);

            /*!
             * \ingroup             file
             * \brief openFileReadOnly Open file read only with mode = 0.
             * \param fileName      The smb url of the file to be opened.
             * \return              Valid file handle, < 0 on error with errno set:
             *                      - ENOMEM  Out of memory
             *                      - EINVAL if an invalid parameter passed, like no
             *                        file, or smbc_init not called.
             *                      - EEXIST  pathname already exists and O_CREAT and
             *                        O_EXCL were used.
             *                      - EISDIR  pathname  refers  to  a  directory  and
             *                        the access requested involved writing.
             *                      - EACCES  The requested access to the file is not
             *                        allowed
             *                      - ENODEV The requested share does not exist
             *                      - ENOTDIR A file on the path is not a directory
             *                      - ENOENT  A directory component in pathname does
             *                        not exist.
             */
            FileHandle openFileReadOnly(const QString& fileName);

            /*!
             * \ingroup             file
             * \brief readFile      Read from a file using an opened file handle.
             *
             * \param handle        Open file handle from openFile().
             *
             * \param buffer        Pointer to buffer to receive read data.
             *
             * \param count         Size of buffer in bytes.
             *
             * \return              Number of bytes read;
             *                      0 upon EOF;
             *                      < 0 on error, with errno set:
             *                      - EISDIR handle refers to a directory
             *                      - EBADF  handle  is  not  a valid file descriptor or
             *                        is not open for reading.
             *                      - EINVAL handle is attached to an object which is
             *                        unsuitable for reading, or no buffer passed or
             *		                  smbc_init not called.
             */
            ssize_t readFile(FileHandle handle, void* buffer, size_t count);

            /*!
             * \ingroup             file
             * \brief writeFile     Write to a file using an opened file handle.
             *
             * @param handle        Open file handle from openFile().
             *
             * @param buffer        Pointer to buffer to recieve read data.
             *
             * @param count         Size of buffer in bytes.
             *
             * @return              Number of bytes written, < 0 on error with errno set:
             *                      - EISDIR handle refers to a directory.
             *                      - EBADF  handle is  not  a valid file descriptor or
             *                        is not open for reading.
             *                      - EINVAL handle is attached to an object which is
             *                        unsuitable for reading, or no buffer passed or
             *		                  smbc_init not called.
             */
            ssize_t writeFile(FileHandle handle, void* buffer, size_t count);

            /*!
             * \ingroup         file
             * \brief seekFile  Seek to a specific location in a file.
             *
             * \param handle    Open file handle from openFile
             *
             * \param offset    Offset in bytes from whence
             *
             * \param whence    A location in the file:
             *                  - SEEK_SET The offset is set to offset bytes from
             *                    the beginning of the file
             *                  - SEEK_CUR The offset is set to current location
             *                    plus offset bytes.
             *                  - SEEK_END The offset is set to the size of the
             *                    file plus offset bytes.
             *
             * \return          Upon successful completion, lseek returns the
             *                  resulting offset location as measured in bytes
             *                  from the beginning  of the file. Otherwise, a value
             *                  of (off_t) - 1 is returned and errno is set to
             *                  indicate the error:
             *                  - EBADF  Fildes is not an open file descriptor.
             *                  - EINVAL Whence is not a proper value or smbc_init
             *		              not called.
             */
            off_t seekFile(FileHandle handle, off_t offset, int whence = SEEK_SET);

            /*!
             * \ingroup         directory
             * \ingroup         file
             * \brief rename    Rename or move a file or directory.
             *
             * \param oldName   The original smb url (source url) of file or
             *                  directory to be moved
             *
             * \param newName   The new smb url (destination url) of the file
             *                  or directory after the move. Currently newName must
             *                  be on the same share as oldName.
             *
             * \return          0 on success, < 0 on error with errno set:
             *                  - EISDIR nurl is an existing directory, but ourl is
             *                    not a directory.
             *                  - EEXIST nurl is  a  non-empty directory,
             *                    i.e., contains entries other than "." and ".."
             *                  - EINVAL The  new  url  contained  a path prefix
             *                    of the old, or, more generally, an  attempt was
             *                    made  to make a directory a subdirectory of itself
             *		              or smbc_init not called.
             *                  - ENOTDIR A component used as a directory in ourl
             *                    or nurl path is not, in fact, a directory.  Or,
             *                    ourl  is a directory, and newpath exists but is not
             *                    a directory.
             *                  - EACCES or EPERM Write access to the directory
             *                    containing ourl or nurl is not allowed for the
             *                    process's effective uid,  or  one of the
             *                    directories in ourl or nurl did not allow search
             *                    (execute) permission,  or ourl  was  a  directory
             *                    and did not allow write permission.
             *                  - ENOENT A  directory component in ourl or nurl
             *                    does not exist.
             *                  - EXDEV Rename across shares not supported.
             *                  - ENOMEM Insufficient kernel memory was available.
             *                  - EEXIST The target file, nurl, already exists.
             */
            int rename(const QString& oldName, const QString &newName);

            /*!
             * \ingroup             file
             * \brief closeFile     Close an open file handle.
             *
             * @param handle        The file handle to close
             *
             * @return              0 on success, < 0 on error with errno set:
             *                      - EBADF  fd isn't a valid open file descriptor
             *                      - EINVAL smbc_init() failed or has not been called
             */
            void closeFile(FileHandle handle);

            /*!
             * \ingroup             stat
             * \brief stat          Get information about a file or directory.
             *
             * \param path          The smb path to get information for.
             *
             * \param st            pointer to a buffer that will be filled with
             *                      standard Unix struct stat information.
             *
             * \return              0 on success, < 0 on error with errno set:
             *                      - ENOENT A component of the path file_name does not
             *                        exist.
             *                      - EINVAL a NULL url was passed or smbc_init not called.
             *                      - EACCES Permission denied.
             *                      - ENOMEM Out of memory
             *                      - ENOTDIR The target dir, url, is not a directory.
             */
            int stat(const QString& path, struct stat& st);


            /*!
             * \ingroup             stat
             * \brief stat          Get file information via an file descriptor.
             *
             * @param handle        Open file handle from smbc_open() or smbc_creat()
             *
             * @param st            pointer to a buffer that will be filled with
             *                      standard Unix struct stat information.
             *
             * @return              0 on success, < 0 on error with errno set:
             *                      - EBADF  filedes is bad.
             *                      - EACCES Permission denied.
             *                      - EBADF fd is not a valid file descriptor
             *                      - EINVAL Problems occurred in the underlying routines
             *		                  or smbc_init not called.
             *                      - ENOMEM Out of memory
             */
            int stat(FileHandle handle, struct stat& st);

            /*!
             * \ingroup             directory
             * \brief openDir       Open a directory used to obtain directory entries.
             *
             * @param directoryName The smb url of the directory to open
             *
             * @return              Valid directory handle. < 0 on error with errno set:
             *                      - EACCES Permission denied.
             *                      - EINVAL A NULL file/URL was passed, or the URL would
             *                        not parse, or was of incorrect form or smbc_init not
             *                        called.
             *                      - ENOENT durl does not exist, or name is an
             *                      - ENOMEM Insufficient memory to complete the
             *                        operation.
             *                      - ENOTDIR name is not a directory.
             *                      - EPERM the workgroup could not be found.
             *                      - ENODEV the workgroup or server could not be found.
             *
             */
            FileHandle openDir(const QString& directoryName);

            /*!
             * \ingroup             directory
             * \brief createDir     Create a directory.
             * \param directoryName The name of the directory to create
             * \param mode          Specifies  the  permissions to use. It is modified
             *                      by the process's umask in the usual way: the
             *                      permissions of the created file are (mode & ~umask).
             * \return              0 on success, < 0 on error with errno set:
             *                      - EEXIST directory already exists
             *                      - EACCES The parent directory does not allow write
             *                        permission to the process, or one of the directories
             *                      - ENOENT A directory component in pathname does not
             *                        exist.
             *                      - EINVAL durl is NULL or smbc_init not called.
             *                      - ENOMEM Insufficient memory was available.
             */            
            int createDir(const QString& directoryName, mode_t mode);

            /*!
             * \ingroup             directory
             * \brief deleteDir     Remove a directory.
             *
             * @param directoryName The smb url of the directory to remove
             *
             * @return              0 on success, < 0 on error with errno set:
             *                      - EACCES or EPERM Write access to the directory
             *                        containing pathname was not allowed.
             *                      - EINVAL durl is NULL or smbc_init not called.
             *                      - ENOENT A directory component in pathname does not
             *                        exist.
             *                      - ENOTEMPTY directory contains entries.
             *                      - ENOMEM Insufficient kernel memory was available.
             */
            int deleteDir(const QString& directoryName);

            /*!
             * \ingroup             attribute
             * \brief               Change the permissions of a file.
             *
             * \param path          The smb url of the file or directory to change
             *                      permissions of
             *
             * \param mode          The permissions to set:
             *                      - Put good explaination of permissions here!
             *
             * \return              0 on success, < 0 on error with errno set:
             *                      - EPERM  The effective UID does not match the owner
             *                        of the file, and is not zero
             *                      - ENOENT The file does not exist.
             *                      - ENOMEM Insufficient was available.
             *                      - ENOENT file or directory does not exist
             */
            bool setPermissions(const QString& path, mode_t mode);

        private:
            static void authenticationCallBack(const char* server, const char* share, char* workGroup,
                                               int workGroupLength, char* user, int userLength, char* password,
                                               int passwordLength);


            inline static ContextPtr createContext(AuthenticationFunction function);
            inline static ContextPtr createContext(const QString &userName, const QString &password,
                                            AuthenticationFunction function);
            inline static ContextPtr createContext(const QString &userName, const QString &password,
                                                   const QString& workGroup, AuthenticationFunction function);

            static void freeContext(ContextPtr context);

        private:
            std::unique_ptr<Context, decltype(&freeContext)> ctx;
        };
    }
}

#endif//GPUI_SMB_CLIENT_H
