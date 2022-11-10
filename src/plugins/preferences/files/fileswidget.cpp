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

#include "fileswidget.h"
#include "ui_fileswidget.h"

#include "common/commonutils.h"
#include "filesitem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace preferences
{
FilesWidget::FilesWidget(QWidget *parent, FilesItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::FilesWidget())
{
    ui->setupUi(this);

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

FilesWidget::~FilesWidget()
{
    delete ui;
}

void FilesWidget::setItem(ModelView::SessionItem *item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->actionComboBox, 0, "currentIndex");
    mapper->addMapping(ui->sourceLineEdit, 1);
    mapper->addMapping(ui->destinationLineEdit, 2);
    mapper->addMapping(ui->supressErrorsCheckBox, 3);
    mapper->addMapping(ui->readOnly, 4);
    mapper->addMapping(ui->archive, 5);
    mapper->addMapping(ui->hidden, 6);

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

bool FilesWidget::validate()
{
    if (!CommonUtils::validateLineEdit(ui->sourceLineEdit, tr("Please enter source file(s) value.")))
    {
        return false;
    }

    if (!CommonUtils::validateLineEdit(ui->destinationLineEdit, tr("Please enter destination file(s) value.")))
    {
        return false;
    }

    return true;
}

QString FilesWidget::name() const
{
    return tr("General");
}

} // namespace preferences
