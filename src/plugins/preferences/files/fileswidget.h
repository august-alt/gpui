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

#ifndef GPUI_FILES_WIDGET_H
#define GPUI_FILES_WIDGET_H

#include <QtWidgets>

#include "common/basepreferencewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FilesWidget; }
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

class FilesItem;
class FilesItemController;

//! Folder item representation for editor.

class FilesWidget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit FilesWidget(QWidget* parent = nullptr, FilesItem* item = nullptr);
    ~FilesWidget() override;

    void setItem(ModelView::SessionItem *item) override;

    bool validate() override;

    QString name() const override;

private slots:
    void on_actionComboBox_currentIndexChanged(int index);
    void on_destinationToolButton_clicked();
    void on_sourceLineEdit_textChanged(const QString &text);
    void on_destinationLineEdit_textChanged(const QString &text);
    void on_sourceToolButton_clicked();

private:
    FilesWidget(const FilesWidget&)            = delete;   // copy ctor
    FilesWidget(FilesWidget&&)                 = delete;   // move ctor
    FilesWidget& operator=(const FilesWidget&) = delete;   // copy assignment
    FilesWidget& operator=(FilesWidget&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    FilesItem* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

    bool fileMode {true};

    size_t whitespaceDetector{0};

private:
    Ui::FilesWidget *ui {nullptr};
};

}

#endif//GPUI_FILES_WIDGET_H
