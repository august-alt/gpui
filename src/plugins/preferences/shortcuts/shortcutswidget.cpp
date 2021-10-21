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

#include "shortcutswidget.h"
#include "ui_shortcutswidget.h"

#include "shortcutroles.h"

namespace gpui
{

class ShortcutsWidgetPrivate
{
public:
    std::unique_ptr<QDataWidgetMapper> mapper;
    QStandardItemModel* model;
    QItemSelectionModel* selectionModel;

    ShortcutsWidgetPrivate(QStandardItemModel* model, QItemSelectionModel* selectionModel)
        : model(model)
        , selectionModel(selectionModel)
    {

    }
};

ShortcutsWidget::ShortcutsWidget(QStandardItemModel& model, QItemSelectionModel& selectionModel, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ShortcutsWidget())
    , d(new ShortcutsWidgetPrivate(&model, &selectionModel))
{
    ui->setupUi(this);

    d->mapper = std::make_unique<QDataWidgetMapper>(this);
    d->mapper->setModel(d->model);

    connect(d->selectionModel, &QItemSelectionModel::currentRowChanged, d->mapper.get(),
            &QDataWidgetMapper::setCurrentModelIndex);

    setupModelMapper();
}

ShortcutsWidget::~ShortcutsWidget()
{
    delete ui;

    delete d;
}

void ShortcutsWidget::setupModelMapper()
{
    d->mapper->addMapping(ui->actionComboBox, ShortcutRoles::ACTION, "currentIndex");
    d->mapper->addMapping(ui->argumentsLineEdit, ShortcutRoles::ARGUMENTS);
    d->mapper->addMapping(ui->commentLineEdit, ShortcutRoles::COMMENT);
    d->mapper->addMapping(ui->iconFilePathLineEdit, ShortcutRoles::ICON_PATH);
    d->mapper->addMapping(ui->iconIndexLineEdit, ShortcutRoles::ICON_INDEX);
    d->mapper->addMapping(ui->shortcutKeyLineEdit, ShortcutRoles::SHORTCUT_KEY);
    d->mapper->addMapping(ui->nameLineEdit, ShortcutRoles::SHORTCUT_PATH);
    d->mapper->addMapping(ui->startInLineEdit, ShortcutRoles::START_IN);
    d->mapper->addMapping(ui->targetPathLineEdit, ShortcutRoles::TARGET_PATH);
    d->mapper->addMapping(ui->targetTypeComboBox, ShortcutRoles::TARGET_TYPE, "currentIndex");

    d->mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

}
