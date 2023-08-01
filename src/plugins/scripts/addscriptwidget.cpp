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
AddScriptWidget::AddScriptWidget(QWidget *parentWidget, ModelView::SessionItem *parentItem, ModelView::SessionItem *item)
    : QDialog(parentWidget)
    , scriptItem(item)
    , scriptParentItem(parentItem)
    , delegate(new ModelView::ViewModelDelegate())
    , mapper(new QDataWidgetMapper())
    , ui(new Ui::AddScriptWidget())
{
    setModal(true);

    this->ui->setupUi(this);

    if (item != nullptr)
    {
        auto currentPath      = this->scriptItem->property<std::string>(ScriptItem::propertyToString(ScriptItem::PATH));
        auto currentArguments = this->scriptItem->property<std::string>(
            ScriptItem::propertyToString(ScriptItem::ARGUMENTS));

        this->ui->nameLineEdit->setText(QString::fromStdString(currentPath));
        this->ui->paramLineEdit->setText(QString::fromStdString(currentArguments));
    }
}

AddScriptWidget::~AddScriptWidget()
{
    delete ui;
}

bool AddScriptWidget::validateState()
{
    return !this->ui->nameLineEdit->text().isEmpty();
}

void AddScriptWidget::on_okPushButton_clicked()
{
    if (!validateState())
    {
        QMessageBox mb(QMessageBox::Warning,
                       QObject::tr("Error while saving script file"),
                       QObject::tr("Script Path cannot be empty"),
                       QMessageBox::Ok);
        mb.exec();
        return;
    }

    if (this->scriptItem == nullptr)
    {
        this->scriptItem = this->scriptParentItem->insertItem<ScriptItem>({"", 0});
    }

    this->scriptItem->setProperty(ScriptItem::propertyToString(ScriptItem::PATH),
                                  ui->nameLineEdit->text().toStdString());
    this->scriptItem->setProperty(ScriptItem::propertyToString(ScriptItem::ARGUMENTS),
                                  ui->paramLineEdit->text().toStdString());

    this->close();
}

void AddScriptWidget::on_cancelPushButton_clicked()
{
    this->close();
}

void AddScriptWidget::on_browsePushButton_clicked()
{
    auto dialog  = std::make_unique<gpui::FileDialogUtils>();
    QString file = dialog->getOpenFileName();

    if (!file.isEmpty())
    {
        ui->nameLineEdit->setText(file);
    }
}

} // namespace scripts_plugin
