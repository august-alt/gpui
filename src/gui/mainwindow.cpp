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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "contentwidget.h"
#include "policydialog.h"

#include "../model/bundle/policybundle.h"

namespace gpui {

class MainWindowPrivate {
public:
    std::unique_ptr<QStandardItemModel> model;
    ContentWidget* contentWidget;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate())
    , ui(new Ui::MainWindow())
{
    ui->setupUi(this);

    d->contentWidget = new ContentWidget(this);

    ui->splitter->addWidget(d->contentWidget);

    connect(ui->actionOpen_Policy_Directory, &QAction::triggered, this, &MainWindow::onDirectoryOpen);
    connect(ui->treeView, &QTreeView::clicked, d->contentWidget, &ContentWidget::modelItemSelected);
}

MainWindow::~MainWindow()
{
    delete d;

    delete ui;
}

void MainWindow::onDirectoryOpen()
{
    QString directory = QFileDialog::getExistingDirectory(
                        this,
                        tr("Open Directory"),
                        QDir::homePath(),
                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    auto bundle = std::make_unique<model::bundle::PolicyBundle>();
    d->model = bundle->loadFolder(directory.toStdString(), "ru-ru", "en-US");

    ui->treeView->setModel(d->model.get());
    d->contentWidget->setModel(d->model.get());

    d->contentWidget->setSelectionModel(ui->treeView->selectionModel());
}

}
