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

#ifndef GPUI_TABLE_DETAILS_WIDGET_H
#define GPUI_TABLE_DETAILS_WIDGET_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui
{
class TableDetailsWidget;
}
QT_END_NAMESPACE

namespace ModelView
{
class ViewModel;
class ViewModelDelegate;
class SessionModel;
} // namespace ModelView

namespace preferences
{
class TableDetailsWidget : public QWidget
{
public:
    Q_OBJECT

public:
    explicit TableDetailsWidget(QWidget *parent = nullptr);
    ~TableDetailsWidget() override;

    void setModel(ModelView::SessionModel *model);

signals:
    void okPressed();

public slots:
    void onItemTypeChange(const std::map<std::string, QString> &newItemTypes);

private slots:
    void on_treeView_customContextMenuRequested(const QPoint &pos);
    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    void setupConnections();

private:
    TableDetailsWidget(const TableDetailsWidget &) = delete;            // copy ctor
    TableDetailsWidget(TableDetailsWidget &&)      = delete;            // move ctor
    TableDetailsWidget &operator=(const TableDetailsWidget &) = delete; // copy assignment
    TableDetailsWidget &operator=(TableDetailsWidget &&) = delete;      // move assignment

private:
    Ui::TableDetailsWidget *ui{nullptr};

private:
    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;
    std::unique_ptr<QDataWidgetMapper> mapper;

    std::map<std::string, QString> itemTypes;
};

} // namespace preferences

#endif //GPUI_TABLE_DETAILS_WIDGET_H
