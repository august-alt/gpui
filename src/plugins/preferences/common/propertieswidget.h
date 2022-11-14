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

#ifndef GPUI_PROPERTIES_WIDGET_H
#define GPUI_PROPERTIES_WIDGET_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui
{
class PropertiesWidget;
}
QT_END_NAMESPACE

namespace ModelView
{
class ViewModel;
class ViewModelDelegate;
class SessionItem;
} // namespace ModelView

namespace preferences
{
class PropertiesWidget : public QWidget
{
public:
    Q_OBJECT

public:
    explicit PropertiesWidget(QWidget *parent = nullptr);
    ~PropertiesWidget() override;

    void setItem(ModelView::SessionItem *item);

    void setDescriptionVisibility(bool visible);

private:
    PropertiesWidget(const PropertiesWidget &) = delete;            // copy ctor
    PropertiesWidget(PropertiesWidget &&)      = delete;            // move ctor
    PropertiesWidget &operator=(const PropertiesWidget &) = delete; // copy assignment
    PropertiesWidget &operator=(PropertiesWidget &&) = delete;      // move assignment

private:
    Ui::PropertiesWidget *ui{nullptr};

private:
    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;
    std::unique_ptr<QDataWidgetMapper> mapper;

private:
    void setupWidgets();
};

} // namespace preferences

#endif //GPUI_PROPERTIES_WIDGET_H
