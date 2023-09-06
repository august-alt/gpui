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

#include <QObject>
#include "../core/isnapinmanagementsettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace gpui {

class MainWindow;
class MainWindowSettingsPrivate;

class MainWindowSettings : public QObject, public ISnapInManagementSettings
{
    Q_OBJECT
public:
    MainWindowSettings(MainWindow *window, Ui::MainWindow *ui);
    ~MainWindowSettings();

    void restoreSettings();

    void saveSettings(QString section, QObject *snapinSettings) override;

public slots:
    void saveSettings();

private:
    MainWindowSettings(const MainWindowSettings&)            = delete;   // copy ctor
    MainWindowSettings(MainWindowSettings&&)                 = delete;   // move ctor
    MainWindowSettings& operator=(const MainWindowSettings&) = delete;   // copy assignment
    MainWindowSettings& operator=(MainWindowSettings&&)      = delete;   // move assignment

private:
    MainWindowSettingsPrivate* d;
};

}

#endif // GPUI_MAIN_WINDOW_SETTINGS_H
