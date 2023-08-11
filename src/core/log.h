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

#define GPUI_DEBUG(__str__) \
    gpui::logger::LoggerManager::getInstance()->logDebug(__str__, __FILE__, __FUNCTION__, __LINE__)

#define GPUI_INFO(__str__) \
    gpui::logger::LoggerManager::getInstance()->logInfo(__str__, __FILE__, __FUNCTION__, __LINE__)

#define GPUI_WARNING(__str__) \
    gpui::logger::LoggerManager::getInstance()->logWarning(__str__, __FILE__, __FUNCTION__, __LINE__)

#define GPUI_ERROR(__str__) \
    gpui::logger::LoggerManager::getInstance()->logError(__str__, __FILE__, __FUNCTION__, __LINE__)

#define GPUI_CRITICAL(__str__) \
    gpui::logger::LoggerManager::getInstance()->logCritical(__str__, __FILE__, __FUNCTION__, __LINE__)

#endif // GPUI_LOG_H
