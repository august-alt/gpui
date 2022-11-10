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

#ifndef GPUI_INI_WIDGET_H
#define GPUI_INI_WIDGET_H

#include <QtWidgets>

#include "common/basepreferencewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class IniWidget; }
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

class IniItem;
class IniItemController;

//! Folder item representation for editor.

class IniWidget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit IniWidget(QWidget* parent = nullptr, IniItem* item = nullptr);
    ~IniWidget() override;

    void setItem(ModelView::SessionItem *item) override;

    bool validate() override;

    QString name() const override;

private slots:
    void on_actionComboBox_currentIndexChanged(int index);
    void on_pathLineEdit_textChanged(const QString &text);
    void on_propertyLineEdit_textChanged(const QString &text);
    void on_sectionLineEdit_textChanged(const QString &text);
    void on_pathToolButton_clicked();

private:
    IniWidget(const IniWidget&)            = delete;   // copy ctor
    IniWidget(IniWidget&&)                 = delete;   // move ctor
    IniWidget& operator=(const IniWidget&) = delete;   // copy assignment
    IniWidget& operator=(IniWidget&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    IniItem* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

private:
    Ui::IniWidget *ui {nullptr};
};

}

#endif//GPUI_INI_WIDGET_H
