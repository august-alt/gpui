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

#ifndef GPUI_SETTINGS_WIDGET_H
#define GPUI_SETTINGS_WIDGET_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsDialog; }
QT_END_NAMESPACE

namespace gpui
{

class ISettingsWidget;

class SettingsDialog : public QDialog
{
public:
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog() override;

    void addTab(ISettingsWidget *settingsWidget);

private slots:
    void on_okButtonClicked();

private:
    SettingsDialog(const SettingsDialog&)            = delete;   // copy ctor
    SettingsDialog(SettingsDialog&&)                 = delete;   // move ctor
    SettingsDialog& operator=(const SettingsDialog&) = delete;   // copy assignment
    SettingsDialog& operator=(SettingsDialog&&)      = delete;   // move assignment

private:
    Ui::SettingsDialog *ui {nullptr};
};

}

#endif//GPUI_SETTINGS_WIDGET_H
