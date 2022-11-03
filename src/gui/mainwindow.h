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

#ifndef GPUI_MAINWINDOW_H
#define GPUI_MAINWINDOW_H

#include "gui.h"

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

namespace model
{
namespace registry
{
class AbstractRegistrySource;
class Registry;
} // namespace registry
} // namespace model

namespace gpui
{
class CommandLineOptions;
class MainWindowPrivate;
class ISnapInManager;
class ISnapIn;

class GPUI_GUI_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    // construction and destruction
    MainWindow(CommandLineOptions &options, ISnapInManager *manager, QWidget *parent = 0);
    ~MainWindow();

    void setLanguage(const QString &language);
    QString getLanguage() const;

    void setAdmxPath(const QString &admxPath);
    QString getAdmxPath() const;

signals:
    void admxPathChanged(const QString &admxPath);

public slots:
    void updateStatusBar();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionManual_triggered();

private:
    MainWindowPrivate *const d;

    Ui::MainWindow *ui;

private slots:
    void onDirectoryOpen();

    void on_actionExit_triggered();
    void on_actionAbout_triggered();

    void onLanguageChanged(QAction *action);

private:
    void onIniFileOpen(const QString &path);

    void loadPolicyModel(ISnapInManager *manager);

    void createLanguageMenu();

private:
    MainWindow(const MainWindow &) = delete;            // copy ctor
    MainWindow(MainWindow &&)      = delete;            // move ctor
    MainWindow &operator=(const MainWindow &) = delete; // copy assignment
    MainWindow &operator=(MainWindow &&) = delete;      // move assignment
};
} // namespace gpui

#endif // GPUI_MAINWINDOW_H
