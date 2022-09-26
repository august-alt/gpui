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

#ifndef GPUI_FOLDER_OPTIONS_WIDGET_H
#define GPUI_FOLDER_OPTIONS_WIDGET_H

#include "common/basepreferencewidget.h"

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class FolderOptionsWidget; }
QT_END_NAMESPACE

class QDataWidgetMapper;

namespace ModelView
{
class ViewModel;
class ViewModelDelegate;
class SessionItem;
}

namespace preferences
{

class FolderOptionsItem;
class FolderOptionsItemController;

//! Folder item representation for editor.

class FolderOptionsWidget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit FolderOptionsWidget(QWidget* parent = nullptr, FolderOptionsItem* item = nullptr);
    ~FolderOptionsWidget() override;

    void setItem(ModelView::SessionItem *item) override;

    QString name() const override;

private:
    FolderOptionsWidget(const FolderOptionsWidget&)            = delete;   // copy ctor
    FolderOptionsWidget(FolderOptionsWidget&&)                 = delete;   // move ctor
    FolderOptionsWidget& operator=(const FolderOptionsWidget&) = delete;   // copy assignment
    FolderOptionsWidget& operator=(FolderOptionsWidget&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    FolderOptionsItem* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

private:
    Ui::FolderOptionsWidget *ui {nullptr};
};

}

#endif//GPUI_FOLDER_OPTIONS_WIDGET_H
