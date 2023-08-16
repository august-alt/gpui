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

#include "filelogger.h"

#include <iostream>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace gpui
{
namespace logger
{
FileLogger::FileLogger(const char *filename)
{
    std::string homeDir = getHomeDir();
    std::string logDir  = homeDir + "/.local/share/gpui";
    ensureDir(logDir.c_str());

    std::string logFile = logDir + filename;
    this->logFileStream.open(logFile, std::fstream::out | std::fstream::app);
}

FileLogger::~FileLogger()
{
    this->logFileStream.close();
}

void FileLogger::logDebug(const LoggerMessage &message)
{
    logMessage("DEBUG", message);
}

void FileLogger::logInfo(const LoggerMessage &message)
{
    logMessage("INFO", message);
}

void FileLogger::logWarning(const LoggerMessage &message)
{
    logMessage("WARNING", message);
}

void FileLogger::logError(const LoggerMessage &message)
{
    logMessage("ERROR", message);
}

void FileLogger::logCritical(const LoggerMessage &message)
{
    logMessage("CRITICAL", message);
}

void FileLogger::logMessage(const std::string &prefix, const LoggerMessage &message)
{
    this->logFileStream << prefix << ": " << message.message << " (" << message.filePath << ":" << message.line << ")"
                        << std::endl;
}

const char *FileLogger::getHomeDir()
{
    const char *homeDir = getenv("HOME");
    if (homeDir == NULL)
    {
        // NOTE(mchernigin): it might be an overkill, checking $HOME should be enough for this use case
        long bufsize = 1 << 14;
        char *buf    = new char[bufsize];
        passwd pwd;
        passwd *result;
        int s = getpwuid_r(getuid(), &pwd, buf, bufsize, &result);
        if (result != NULL)
        {
            homeDir = result->pw_dir;
        }
        else
        {
            homeDir = "/root";
            std::cerr << "FileLogger Error: cannot determine home directory, defaulting to " << homeDir;
        }

        delete[] buf;
    }

    return homeDir;
}

bool FileLogger::ensureDir(const char *path)
{
    struct stat sb;
    if (stat(path, &sb) != 0 && mkdir(path, 0750) != 0)
    {
        std::cerr << "Cannot create log directory (" << path << ")\n";
        return false;
    }

    return true;
}
} // namespace logger
} // namespace gpui
