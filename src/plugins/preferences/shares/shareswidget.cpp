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

#include "shareswidget.h"
#include "ui_shareswidget.h"

#include "sharesitem.h"
#include "common/commonutils.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace  preferences
{

SharesWidget::SharesWidget(QWidget *parent, SharesItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::SharesWidget())
{
    ui->setupUi(this);

    if (m_item)
    {
        setItem(item);
    }

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

SharesWidget::~SharesWidget()
{
    delete ui;
}

void SharesWidget::setItem(ModelView::SessionItem* item)
{
    m_item = dynamic_cast<SharesItem*>(item);

    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->actionComboBox, 0);
    mapper->addMapping(ui->shareNameLineEdit, 1);
    mapper->addMapping(ui->folderPathLineEdit, 2);
    mapper->addMapping(ui->commentLineEdit, 3);
    mapper->addMapping(ui->updateAllRegularShares, 4);
    mapper->addMapping(ui->updateAllHiddenShares, 5);
    mapper->addMapping(ui->updateAllAdministrativeDrives, 6);
    mapper->addMapping(ui->numberOfUsers, 8);

    mapper->setCurrentModelIndex(view_model->index(0, 1));

    setInitialUserFrameRadioButton(view_model->index(7, 1).data().toString());
    setInitialAccessFrameRadioButton(view_model->index(9, 1).data().toString());

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

bool SharesWidget::validate()
{
    if (!CommonUtils::validateLineEdit(ui->shareNameLineEdit, tr("Please input name value.")))
    {
        return false;
    }

    if (!CommonUtils::validateLineEdit(ui->folderPathLineEdit, tr("Please input folder path value.")))
    {
        return false;
    }

    return true;
}

QString SharesWidget::name() const
{
    return "General";
}

}
