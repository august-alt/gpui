/***********************************************************************************************************************
**
** Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>
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

#include "snapinmanagementwidget.h"
#include "ui_snapinmanagementwidget.h"

#include "isnapin.h"
#include "snapindetailsfactory.h"

#include <QTableWidget>
#include <QTreeWidgetItem>

#include <QMessageBox>

#include <memory>

Q_DECLARE_METATYPE(::gpui::ISnapIn *)

namespace gpui
{
enum TreeItemColumn
{
    FIRST   = 0,
    ENABLED = 0,
    NAME    = 1,
    VERSION = 2,
};

class SnapInManagementWidgetPrivate
{
public:
    Ui::SnapInManagementWidget *ui                = nullptr;
    ISnapInManager *manager                       = nullptr;
    std::unique_ptr<SnapInDetailsFactory> factory = nullptr;

    SnapInManagementWidgetPrivate()
        : ui(new Ui::SnapInManagementWidget())
        , factory(new SnapInDetailsFactory())
    {}

    ~SnapInManagementWidgetPrivate() { delete ui; }

private:
    SnapInManagementWidgetPrivate(const SnapInManagementWidgetPrivate &) = delete; // copy ctor
    SnapInManagementWidgetPrivate(SnapInManagementWidgetPrivate &&)      = delete; // move ctor
    SnapInManagementWidgetPrivate &operator=(const SnapInManagementWidgetPrivate &)
        = delete; // copy assignment
    SnapInManagementWidgetPrivate &operator=(SnapInManagementWidgetPrivate &&)
        = delete; // move assignment
};

SnapInManagementWidget::SnapInManagementWidget(QWidget *parent, ISnapInManager *manager)
    : QWidget(parent)
    , d(new SnapInManagementWidgetPrivate())
{
    d->manager = manager;
    d->ui->setupUi(this);

    for (ISnapIn *snapIn : manager->getSnapIns())
    {
        auto snapInItem = new QTreeWidgetItem();

        auto enabled     = true;
        auto version     = snapIn->getVersion();
        auto displayName = snapIn->getDisplayName();

        snapInItem->setText(ENABLED, enabled ? "Yes" : "No");
        snapInItem->setText(NAME, displayName);
        snapInItem->setText(VERSION, version.toString());

        snapInItem->setData(FIRST, Qt::UserRole, QVariant::fromValue(snapIn));

        d->ui->treeWidget->addTopLevelItem(snapInItem);
    }

    d->ui->treeWidget->expandAll();
}

SnapInManagementWidget::~SnapInManagementWidget()
{
    delete d;
}

void SnapInManagementWidget::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    if (!item)
    {
        return;
    }

    auto snapIn = item->data(FIRST, Qt::UserRole).value<ISnapIn *>();

    if (snapIn)
    {
        try
        {
            auto snapInDetailsDialog = d->factory->create(snapIn->getType());
            if (snapInDetailsDialog)
            {
                snapInDetailsDialog->setSnapIn(snapIn);
                snapInDetailsDialog->exec();
            }
        }
        catch (const std::exception &error)
        {
            QMessageBox mb(QMessageBox::Critical,
                           "Error while creating dialog widget",
                           error.what(),
                           QMessageBox::Ok);
            mb.exec();
        }
    }
}

} // namespace gpui
