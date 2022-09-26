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

#ifndef GPUI_DRIVES_WIDGET_H
#define GPUI_DRIVES_WIDGET_H

#include <QtWidgets>

#include "common/basepreferencewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DrivesWidget; }
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

class DrivesItem;
class DrivesItemController;

//! Folder item representation for editor.

class DrivesWidget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    enum DrivesWidgetMode
    {
        CREATE       = 0,
        USE_EXISTING = 1,
        DELETE       = 3
    };

public:
    explicit DrivesWidget(QWidget* parent = nullptr, DrivesItem* item = nullptr);
    ~DrivesWidget() override;

    void setItem(ModelView::SessionItem *item) override;

    QString name() const override;

public slots:
    void submit() override;

private slots:
    void on_actionComboBox_currentIndexChanged(int index);
    void on_pathLineEdit_textChanged(const QString &text);
    void on_pathToolButton_clicked();

private:
    void setDriveRadioButtonText(DrivesWidgetMode mode);
    void disableLetterWidgets(const QString& text, int index);
    void setThisDriveCheckBox(const int index);
    void setAllDrivesCheckBox(const int index);

private:
    DrivesWidget(const DrivesWidget&)            = delete;   // copy ctor
    DrivesWidget(DrivesWidget&&)                 = delete;   // move ctor
    DrivesWidget& operator=(const DrivesWidget&) = delete;   // copy assignment
    DrivesWidget& operator=(DrivesWidget&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    DrivesItem* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

private:
    Ui::DrivesWidget *ui {nullptr};
};

}

#endif//GPUI_DRIVES_WIDGET_H
