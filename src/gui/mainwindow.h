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
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace model {
    namespace registry {
        class AbstractRegistrySource;
        class Registry;
    }
}

namespace gpui {

    class CommandLineOptions;
    class MainWindowPrivate;

    class GPUI_GUI_EXPORT MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        // construction and destruction
        MainWindow(CommandLineOptions& options, QWidget *parent = 0);
        ~MainWindow();

    protected:
        void closeEvent(QCloseEvent *event) override;

    private:

        MainWindowPrivate* const d;

        Ui::MainWindow *ui;

    private slots:
        void onDirectoryOpen();
        void onRegistrySourceSave();

        void on_actionExit_triggered();

        void onLanguageChanged(QAction *action);

    private:
        void onPolFileOpen(const QString& path,
                           std::shared_ptr<model::registry::Registry>& registry,
                           std::unique_ptr<model::registry::AbstractRegistrySource>& source,
                           std::function<void(model::registry::AbstractRegistrySource* source)> callback);

        void loadPolicyBundleFolder(const QString& path, const QString& locale);

        void createLanguageMenu();
    };
}

#endif // GPUI_MAINWINDOW_H
