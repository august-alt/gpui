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

#ifndef GPUI_FILEEXTENSION_WIDGET_H
#define GPUI_FILEEXTENSION_WIDGET_H

#include "common/basepreferencewidget.h"

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class FileExtensionWidget; }
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

class FileExtensionItem;

//! FileExtension representation for editor.

class FileExtensionWidget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit FileExtensionWidget(QWidget* parent = nullptr, FileExtensionItem* item = nullptr);
    ~FileExtensionWidget() override;

    void setItem(ModelView::SessionItem *item) override;

    virtual QString name() const override;

private slots:
    void on_actionComboBox_currentIndexChanged(int index);

private:
    FileExtensionWidget(const FileExtensionWidget&)            = delete;   // copy ctor
    FileExtensionWidget(FileExtensionWidget&&)                 = delete;   // move ctor
    FileExtensionWidget& operator=(const FileExtensionWidget&) = delete;   // copy assignment
    FileExtensionWidget& operator=(FileExtensionWidget&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    FileExtensionItem* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

private:
    Ui::FileExtensionWidget *ui {nullptr};
};

}

#endif//GPUI_FILEEXTENSION_WIDGET_H

