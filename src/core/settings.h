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

#ifndef GPUI_MAIN_WINDOW_SETTINGS_H
#define GPUI_MAIN_WINDOW_SETTINGS_H

#include "../core/isnapinmanagementsettings.h"
#include <QObject>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

namespace gpui
{
class MainWindow;
class SettingsPrivate;

class GPUI_CORE_EXPORT Settings : public QObject, public ISnapInManagementSettings
{
    Q_OBJECT
public:
    Settings();
    ~Settings();

    void saveSettings(QString section, QObject *snapinSettings) override;

    void loadSettings(QString section, QObject *snapinSettings) override;

private:
    Settings(const Settings &) = delete;            // copy ctor
    Settings(Settings &&)      = delete;            // move ctor
    Settings &operator=(const Settings &) = delete; // copy assignment
    Settings &operator=(Settings &&) = delete;      // move assignment

private:
    SettingsPrivate *d;
};

} // namespace gpui

#endif // GPUI_MAIN_WINDOW_SETTINGS_H
