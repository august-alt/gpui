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

#include "tcpprintergeneralwidget.h"
#include "ui_tcpprintergeneralwidget.h"

#include "tcpprinteritem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace preferences
{
TcpPrinterGeneralWidget::TcpPrinterGeneralWidget(QWidget *parent, TcpPrinterItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::TcpPrinterGeneralWidget())
{
    ui->setupUi(this);

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

TcpPrinterGeneralWidget::~TcpPrinterGeneralWidget()
{
    delete ui;
}

void TcpPrinterGeneralWidget::setItem(ModelView::SessionItem *item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->actionComboBox, TcpPrinterItem::propertyToInt(TcpPrinterItem::ACTION), "currentIndex");
    mapper->addMapping(ui->deleteCheckBox, TcpPrinterItem::propertyToInt(TcpPrinterItem::DELETE_ALL));
    mapper->addMapping(ui->ipaddressLineEdit, TcpPrinterItem::propertyToInt(TcpPrinterItem::IP_ADDRESS));
    mapper->addMapping(ui->usednsCheckBox, TcpPrinterItem::propertyToInt(TcpPrinterItem::USE_DNS));
    mapper->addMapping(ui->localNameLineEdit, TcpPrinterItem::propertyToInt(TcpPrinterItem::LOCAL_NAME));
    mapper->addMapping(ui->pathLineEdit, TcpPrinterItem::propertyToInt(TcpPrinterItem::PATH));
    mapper->addMapping(ui->defaultCheckBox, TcpPrinterItem::propertyToInt(TcpPrinterItem::DEFAULT));
    mapper->addMapping(ui->skipLocalCheckBox, TcpPrinterItem::propertyToInt(TcpPrinterItem::SKIP_LOCAL));
    mapper->addMapping(ui->locationLineEdit, TcpPrinterItem::propertyToInt(TcpPrinterItem::LOCATION));
    mapper->addMapping(ui->commentLineEdit, TcpPrinterItem::propertyToInt(TcpPrinterItem::COMMENT));

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

void TcpPrinterGeneralWidget::on_actionComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
}

QString TcpPrinterGeneralWidget::name() const
{
    return "General";
}

} // namespace preferences
