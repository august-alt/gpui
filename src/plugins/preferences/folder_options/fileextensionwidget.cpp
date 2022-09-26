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

#include "fileextensionwidget.h"
#include "ui_fileextensionwidget.h"

#include "fileextensionitem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace  preferences
{

FileExtensionWidget::FileExtensionWidget(QWidget *parent, FileExtensionItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::FileExtensionWidget())
{
    ui->setupUi(this);

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

FileExtensionWidget::~FileExtensionWidget()
{
    delete ui;
}

void FileExtensionWidget::setItem(ModelView::SessionItem* item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->actionComboBox, FileExtensionItem::propertyToInt(FileExtensionItem::ACTION));
    mapper->addMapping(ui->fileExtentionLineEdit, FileExtensionItem::propertyToInt(FileExtensionItem::FILE_EXT));
    mapper->addMapping(ui->classComboBox, FileExtensionItem::propertyToInt(FileExtensionItem::APPLICATION));
    mapper->addMapping(ui->classComboBox, FileExtensionItem::propertyToInt(FileExtensionItem::APP_PROG_ID));
    mapper->addMapping(ui->settingsCheckBox, FileExtensionItem::propertyToInt(FileExtensionItem::CONFIG_ACTIONS));
    mapper->addMapping(ui->iconPathLineEdit, FileExtensionItem::propertyToInt(FileExtensionItem::ICON_PATH));
    mapper->addMapping(ui->iconIndexLineEdit, FileExtensionItem::propertyToInt(FileExtensionItem::ICON_INDEX));
    mapper->addMapping(ui->openAftercheckBox, FileExtensionItem::propertyToInt(FileExtensionItem::CONFIRM_OPEN));
    mapper->addMapping(ui->alwaysCheckBox, FileExtensionItem::propertyToInt(FileExtensionItem::ALWAYS_SHOW));
    mapper->addMapping(ui->browseCheckBox, FileExtensionItem::propertyToInt(FileExtensionItem::SAME_WINDOW));

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

QString FileExtensionWidget::name() const
{
    return "File Extenision";
}

void FileExtensionWidget::on_actionComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
}

}

