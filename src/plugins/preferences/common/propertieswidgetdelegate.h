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

#ifndef GPUI_COMMONVIEW_DELEGATE_H
#define GPUI_COMMONVIEW_DELEGATE_H

#include <mvvm/viewmodel/viewmodeldelegate.h>

namespace preferences
{
class PropertiesWidgetDelegate : public ModelView::ViewModelDelegate
{
public:
    Q_OBJECT

public:
    explicit PropertiesWidgetDelegate(QWidget *parent = nullptr);
    ~PropertiesWidgetDelegate() override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

private:
    PropertiesWidgetDelegate(const PropertiesWidgetDelegate &) = delete;            // copy ctor
    PropertiesWidgetDelegate(PropertiesWidgetDelegate &&)      = delete;            // move ctor
    PropertiesWidgetDelegate &operator=(const PropertiesWidgetDelegate &) = delete; // copy assignment
    PropertiesWidgetDelegate &operator=(PropertiesWidgetDelegate &&) = delete;      // move assignment
};

} // namespace preferences

#endif //GPUI_COMMONVIEW_DELEGATE_H
