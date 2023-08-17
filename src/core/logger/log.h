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

#ifndef GPUI_LOG_H
#define GPUI_LOG_H

#include "loggermanager.h"
#include "loggerstream.h"

#define GPUI_DEBUG(message) \
    gpui::logger::LoggerManager::getInstance()->logDebug(message, __FILE__, __FUNCTION__, __LINE__)

#define GPUI_INFO(message) \
    gpui::logger::LoggerManager::getInstance()->logInfo(message, __FILE__, __FUNCTION__, __LINE__)

#define GPUI_WARNING(message) \
    gpui::logger::LoggerManager::getInstance()->logWarning(message, __FILE__, __FUNCTION__, __LINE__)

#define GPUI_ERROR(message) \
    gpui::logger::LoggerManager::getInstance()->logError(message, __FILE__, __FUNCTION__, __LINE__)

#define GPUI_CRITICAL(message) \
    gpui::logger::LoggerManager::getInstance()->logCritical(message, __FILE__, __FUNCTION__, __LINE__)

#define GPUI_DEBUG_STREAM \
    gpui::logger::LoggerStream(gpui::logger::LoggerManager::getInstance(), \
                               gpui::logger::Debug, \
                               __FILE__, \
                               __FUNCTION__, \
                               __LINE__)

#define GPUI_INFO_STREAM \
    gpui::logger::LoggerStream(gpui::logger::LoggerManager::getInstance(), \
                               gpui::logger::Info, \
                               __FILE__, \
                               __FUNCTION__, \
                               __LINE__)

#define GPUI_WARNING_STREAM \
    gpui::logger::LoggerStream(gpui::logger::LoggerManager::getInstance(), \
                               gpui::logger::Warning, \
                               __FILE__, \
                               __FUNCTION__, \
                               __LINE__)

#define GPUI_ERROR_STREAM \
    gpui::logger::LoggerStream(gpui::logger::LoggerManager::getInstance(), \
                               gpui::logger::Error, \
                               __FILE__, \
                               __FUNCTION__, \
                               __LINE__)

#define GPUI_CRITICAL_STREAM \
    gpui::logger::LoggerStream(gpui::logger::LoggerManager::getInstance(), \
                               gpui::logger::Critical, \
                               __FILE__, \
                               __FUNCTION__, \
                               __LINE__)

#endif // GPUI_LOG_H
