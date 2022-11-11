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

#include "scriptscontentwidget.h"
#include "ui_scriptscontentwidget.h"

#include "scriptsdialog.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/viewmodel/viewitem.h>
#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace scripts_plugin
{
ScriptsContentWidget::ScriptsContentWidget(QWidget *parent, ModelView::SessionModel *newSourceModel)
    : QWidget(parent)
    , sourceModel(nullptr)
    , viewModel(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , mapper(nullptr)
    , ui(new Ui::ScriptsContentWidget())
{
    ui->setupUi(this);

    Q_UNUSED(newSourceModel);
}

ScriptsContentWidget::~ScriptsContentWidget()
{
    delete ui;
}

void ScriptsContentWidget::setItem(ModelView::SessionItem *item)
{
    if (!item)
    {
        qWarning() << "No item!";
    }

    viewModel = ModelView::Factory::CreateTopItemsViewModel(item->model());
    viewModel->setRootSessionItem(item);

    ui->listView->setModel(viewModel.get());

    ui->listView->setItemDelegate(delegate.get());
}

void ScriptsContentWidget::setSelectionModel(QItemSelectionModel *newSelectionModel)
{
    Q_UNUSED(newSelectionModel);
}

void ScriptsContentWidget::on_listView_doubleClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        auto item = viewModel->sessionItemFromIndex(index);
        if (item->model()->modelType().compare("ScriptsModel") == 0)
        {
            ScriptsDialog(item, item, this).exec();
        }
        else
        {
            setItem(viewModel->itemFromIndex(index)->item());
        }
    }
}

void ScriptsContentWidget::on_buttonBox_accepted()
{
    emit onAccepted();
}

} // namespace scripts_plugin
