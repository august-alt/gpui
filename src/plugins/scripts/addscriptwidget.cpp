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

#include "addscriptwidget.h"
#include "ui_addscriptwidget.h"

#include <gui/filedialogutils.h>
#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include "scriptitem.h"

#include <QDataWidgetMapper>

#include <QDebug>

namespace scripts_plugin
{
AddScriptWidget::AddScriptWidget(QWidget *parent)
    : QDialog(parent)
    , m_item(nullptr)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , mapper(std::make_unique<QDataWidgetMapper>())
    , ui(new Ui::AddScriptWidget())
{
    ui->setupUi(this);
}

AddScriptWidget::~AddScriptWidget()
{
    delete ui;
}

void AddScriptWidget::setItem(ModelView::SessionItem *item)
{
    m_item          = item;
    ScriptItem *var = dynamic_cast<ScriptItem *>(item);

    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->nameLineEdit, 0);
    mapper->addMapping(ui->paramLineEdit, 1);

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

void AddScriptWidget::setDeletingFlag(bool flag)
{
    deletingFlag = flag;
}

void AddScriptWidget::on_okPushButton_clicked()
{
    if (ui->nameLineEdit->text().isEmpty())
    {
        if (deletingFlag)
        {
            auto model = m_item->parent()->model();
            model->removeItem(m_item->parent(), m_item->tagRow());
        }
    }
    else
    {
        mapper->submit();
    }

    this->close();
}

void AddScriptWidget::on_cancelPushButton_clicked()
{
    this->close();
}

void AddScriptWidget::on_browsePushButton_clicked()
{
    auto dialog  = new gpui::FileDialogUtils;
    QString file = dialog->getOpenFileName();

    if (!file.isEmpty())
    {
        ui->nameLineEdit->setText(file);
    }

    delete dialog;
}

} // namespace scripts_plugin
