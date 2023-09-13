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

#ifndef GPUI_MAINWINDOW_SETTINGS_H
#define GPUI_MAINWINDOW_SETTINGS_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../core/settings.h"

#include <QObject>

namespace gpui
{

class MainWindowSettingsPrivate;

class MainWindowSettings : public QObject
{
public:
    Q_OBJECT

    Q_PROPERTY(QByteArray geometry MEMBER geometry)
    Q_PROPERTY(QByteArray windowState MEMBER windowState)
    Q_PROPERTY(QByteArray splitterState MEMBER splitterState)
    Q_PROPERTY(QString languageState MEMBER languageState)
    Q_PROPERTY(QString admxPath MEMBER admxPath)

public:
    MainWindowSettings(MainWindow *window, Ui::MainWindow *ui, Settings *setttings);
    ~MainWindowSettings();

    void saveSettings();
    void loadSettings();

public:
    QByteArray geometry{} ;
    QByteArray windowState{};
    QByteArray splitterState{};
    QString languageState{};
    QString admxPath{};

private:
    MainWindowSettingsPrivate *d;

private:
    void convertSettingsToProperties();
    void convertPropertiesToSettings();

    MainWindowSettings(const MainWindowSettings&) = delete;
    MainWindowSettings(MainWindowSettings &&) = delete;
    MainWindowSettings &operator=(const MainWindowSettings &) = delete;
    MainWindowSettings &operator=(MainWindowSettings &&) = delete;
};

} //gpui

#endif // MAINWINDOWSETTINGS_H
