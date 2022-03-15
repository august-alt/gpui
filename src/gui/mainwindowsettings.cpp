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

#include "mainwindowsettings.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

namespace gpui {

const QString MAIN_WINDOW_GEOMETRY = "mainwindow/geometry";
const QString MAIN_WINDOW_STATE = "mainwindow/state";
const QString MAIN_WINDOW_SPLITTER_STATE = "mainwindow/splitterState";

const QString MAIN_WINDOW_LANGUAGE_STATE = "mainwindow/language";
const QString MAIN_WINDOW_ADMX_PATH = "mainwindow/admxPath";

class MainWindowSettingsPrivate
{
public:
    Ui::MainWindow* ui = nullptr;
    QSettings settings {};
    gpui::MainWindow* window = nullptr;
};

MainWindowSettings::MainWindowSettings(gpui::MainWindow* window, Ui::MainWindow* ui)
    : d(new MainWindowSettingsPrivate())
{
    d->window = window;
    d->ui = ui;
}

MainWindowSettings::~MainWindowSettings()
{
    delete d;
}

void MainWindowSettings::saveSettings()
{
    const QByteArray geometry = d->window->saveGeometry();
    d->settings.setValue(MAIN_WINDOW_GEOMETRY, geometry);

    const QByteArray state = d->window->saveState();
    d->settings.setValue(MAIN_WINDOW_STATE, state);

    const QByteArray splitterState = d->ui->splitter->saveState();
    d->settings.setValue(MAIN_WINDOW_SPLITTER_STATE, splitterState);

    const QString& languageState = d->window->getLanguage();
    d->settings.setValue(MAIN_WINDOW_LANGUAGE_STATE, languageState);

    const QString& admxPath = d->window->getAdmxPath();
    d->settings.setValue(MAIN_WINDOW_ADMX_PATH, admxPath);
}

void MainWindowSettings::restoreSettings()
{
    const QByteArray geometry = d->settings.value(MAIN_WINDOW_GEOMETRY).toByteArray();
    d->window->restoreGeometry(geometry);

    const QByteArray state = d->settings.value(MAIN_WINDOW_STATE).toByteArray();
    d->window->restoreState(state);

    const QByteArray splitterState = d->settings.value(MAIN_WINDOW_SPLITTER_STATE).toByteArray();
    d->ui->splitter->restoreState(splitterState);

    const QString languageState = d->settings.value(MAIN_WINDOW_LANGUAGE_STATE).toString();
    d->window->setLanguage(languageState);

    const QString admxPath = d->settings.value(MAIN_WINDOW_ADMX_PATH).toString();
    d->window->setAdmxPath(admxPath);
}

}
