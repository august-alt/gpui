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

#ifndef BASESCRIPTSWIDGET_H
#define BASESCRIPTSWIDGET_H

#include "groupscriptcontaineritem.h"

#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <mvvm/factories/viewmodelfactory.h>

#include <QHeaderView>

namespace scripts_plugin
{
class BaseScriptWidget
{
public:
    BaseScriptWidget();
    virtual ~BaseScriptWidget();

    BaseScriptWidget(const BaseScriptWidget &) = delete;            //copy ctor
    BaseScriptWidget(BaseScriptWidget &&)      = delete;            // mode ctor
    BaseScriptWidget &operator=(const BaseScriptWidget &) = delete; //copy assignment
    BaseScriptWidget &operator=(BaseScriptWidget &&) = delete;      //mode assignment

public:
    std::unique_ptr<ModelView::ViewModel> model = nullptr;
    ModelView::SessionModel *sessionModel       = nullptr;
    ModelView::SessionItem *rootItem            = nullptr;

    ModelView::ViewItem *selectedItem = nullptr;

    template<typename TUi>
    void setItem(TUi *ui, GroupScriptContainerItem *item)
    {
        rootItem = item;

        sessionModel = item->model();

        model = ModelView::Factory::CreatePropertyTableViewModel(item->model());

        model->setRootSessionItem(item);

        ui->treeView->setModel(model.get());

        setupConnections(ui);
    }

    template<typename TUi>
    void setupConnections(TUi *ui)
    {
        QObject::connect(ui->treeView->selectionModel(),
                         &QItemSelectionModel::selectionChanged,
                         [&](const QItemSelection &selected, const QItemSelection &deselected) {
                             Q_UNUSED(deselected);

                             if (model && selected.indexes().size() > 0)
                             {
                                 QModelIndex selectedIndex = selected.indexes().at(0);
                                 selectedItem              = model->itemFromIndex(selectedIndex);
                             }
                         });
    }
};
} // namespace scripts_plugin

#endif // BASESCRIPTSWIDGET_H
