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
#include "scriptitemcontainer.h"

#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <mvvm/factories/viewmodelfactory.h>

#include <QHeaderView>
#include <QTreeView>

namespace scripts_plugin
{
class BaseScriptTabWidget
{
public:
    BaseScriptTabWidget(QWidget *p);
    virtual ~BaseScriptTabWidget();

    BaseScriptTabWidget(const BaseScriptTabWidget &) = delete;            //copy ctor
    BaseScriptTabWidget(BaseScriptTabWidget &&)      = delete;            // mode ctor
    BaseScriptTabWidget &operator=(const BaseScriptTabWidget &) = delete; //copy assignment
    BaseScriptTabWidget &operator=(BaseScriptTabWidget &&) = delete;      //mode assignment

public:
    void onUpClicked();
    void onDownClicked();
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();
    void onBrowseClicked();

public:
    std::unique_ptr<ModelView::ViewModel> model = nullptr;
    ModelView::SessionModel *sessionModel       = nullptr;
    ModelView::SessionItem *rootItem            = nullptr;
    ScriptItemContainer *scriptsItemContainer   = nullptr;

    ModelView::ViewItem *selectedItem = nullptr;

    QTreeView* treeView = nullptr;

    bool isStartUpScripts = false;

    template<typename TUi>
    void setItem(TUi *ui, ScriptItemContainer *item, bool startUpScriptsFlag)
    {
        isStartUpScripts = startUpScriptsFlag;

        scriptsItemContainer = item;

        rootItem = item->getScripts();

        sessionModel = rootItem->model();

        model = ModelView::Factory::CreatePropertyTableViewModel(rootItem->model());

        model->setRootSessionItem(rootItem);

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

private:
    QWidget *parent;
};
} // namespace scripts_plugin

#endif // BASESCRIPTSWIDGET_H
