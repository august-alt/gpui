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

#ifndef MVVM_FOLDERS_ADDSCRIPTS_WIDGET_H
#define MVVM_FOLDERS_ADDSCRIPTS_WIDGET_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui
{
class AddScriptWidget;
}
QT_END_NAMESPACE

class QDataWidgetMapper;

namespace ModelView
{
class ViewModel;
class ViewModelDelegate;
class SessionItem;
} // namespace ModelView

namespace scripts_plugin
{
class AddScriptWidget : public QDialog
{
public:
    Q_OBJECT

public:
    explicit AddScriptWidget(QWidget *parent = nullptr);
    ~AddScriptWidget();

    void setItem(ModelView::SessionItem *item);

    void setDeletingFlag(bool flag);

private slots:
    void on_okPushButton_clicked();

    void on_cancelPushButton_clicked();

    void on_browsePushButton_clicked();

private:
    AddScriptWidget(const AddScriptWidget &) = delete;            // copy ctor
    AddScriptWidget(AddScriptWidget &&)      = delete;            // move ctor
    AddScriptWidget &operator=(const AddScriptWidget &) = delete; // copy assignment
    AddScriptWidget &operator=(AddScriptWidget &&) = delete;      // move assignment

private:
    //!< Underlying item of this view.
    ModelView::SessionItem *m_item{nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;
    std::unique_ptr<QDataWidgetMapper> mapper;

    bool deletingFlag = false;

private:
    Ui::AddScriptWidget *ui{nullptr};
};

} // namespace scripts_plugin

#endif //MVVM_FOLDERS_ADDSCRIPTS_WIDGET_H
