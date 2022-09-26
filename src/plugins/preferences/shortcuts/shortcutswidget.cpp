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

#include "common/commonutils.h"
#include "shortcutsitem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace preferences
{
ShortcutsWidget::ShortcutsWidget(QWidget *parent, ShortcutsItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::ShortcutsWidget())
{
    ui->setupUi(this);

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

ShortcutsWidget::~ShortcutsWidget()
{
    delete ui;
}

void ShortcutsWidget::setItem(ModelView::SessionItem *item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->actionComboBox, 0);
    mapper->addMapping(ui->nameLineEdit, 2);
    mapper->addMapping(ui->targetTypeComboBox, 3);
    mapper->addMapping(ui->targetPathLineEdit, 4);
    mapper->addMapping(ui->argumentsLineEdit, 5);
    mapper->addMapping(ui->startInLineEdit, 6);
    //    mapper->addMapping(ui->shortkutKeySequenceEdit, 7);
    mapper->addMapping(ui->runComboBox, 8);
    mapper->addMapping(ui->commentLineEdit, 9);
    mapper->addMapping(ui->iconFilePathLineEdit, 10);
    mapper->addMapping(ui->iconIndexLineEdit, 11);

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

bool ShortcutsWidget::validate()
{
    if (!CommonUtils::validateLineEdit(ui->nameLineEdit, tr("Please input name value.")))
    {
        return false;
    }

    if (!CommonUtils::validateLineEdit(ui->targetPathLineEdit,
                                       tr("Please input target path value.")))
    {
        return false;
    }

    return true;
}

QString ShortcutsWidget::name() const
{
    return "General";
}

} // namespace preferences
