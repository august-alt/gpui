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

#include "basescripttabwidget.h"
#include "addscriptwidget.h"

#include <mvvm/model/modelutils.h>
#include <mvvm/viewmodel/viewitem.h>
#include <QDebug>
#include <QDesktopServices>

namespace scripts_plugin
{
BaseScriptTabWidget::BaseScriptTabWidget(QWidget *p)
    : parent(p)
{}

void BaseScriptTabWidget::onUpClicked()
{
    if (this->selectedItem != nullptr)
    {
        ModelView::Utils::MoveUp(this->selectedItem->item()->parent());
    }
}

void BaseScriptTabWidget::onDownClicked()
{
    if (this->selectedItem != nullptr)
    {
        ModelView::Utils::MoveDown(this->selectedItem->item()->parent());
    }
}

void BaseScriptTabWidget::onAddClicked()
{
    auto addWidget = new AddScriptWidget(parent, this->rootItem, nullptr);
    addWidget->setWindowTitle(QObject::tr("Add script"));
    addWidget->show();
}

void BaseScriptTabWidget::onEditClicked()
{
    if (this->selectedItem != nullptr)
    {
        auto editWidget = new AddScriptWidget(parent, nullptr, this->selectedItem->item()->parent());
        editWidget->setWindowTitle(QObject::tr("Edit script"));
        editWidget->show();
    }
}

void BaseScriptTabWidget::onDeleteClicked()
{
    auto *item = this->selectedItem;
    if (item != nullptr)
    {
        if (this->selectedItem && this->selectedItem->item()->parent())
        {
            auto parentItem = this->selectedItem->item()->parent();

            this->sessionModel->removeItem(parentItem->parent(), parentItem->tagRow());

            this->selectedItem = nullptr;
        }
        else
        {
            qWarning() << "Selected item:" << this->selectedItem << "Parent:" << this->selectedItem->item()->parent();
        }
    }
}

void BaseScriptTabWidget::onBrowseClicked()
{
    if (!scriptsItemContainer)
    {
        return;
    }

    auto path = scriptsItemContainer->property<std::string>(ScriptItemContainer::INI_FILE_PATH);

    QString dirName = QFileInfo(QString::fromStdString(path)).absolutePath();

    qWarning() << dirName;

    QDesktopServices::openUrl(QUrl(dirName, QUrl::TolerantMode));
}

BaseScriptTabWidget::~BaseScriptTabWidget() {}

} // namespace scripts_plugin
