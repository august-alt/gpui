/***********************************************************************************************************************
**
** Copyright (C) 2024 BaseALT Ltd. <org@basealt.ru>
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

#ifndef GPUI_SELECT_VARIABLE_WIDGET_H
#define GPUI_SELECT_VARIABLE_WIDGET_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui
{
class SelectVariableWidget;
}
QT_END_NAMESPACE

namespace preferences
{
class SelectVariableWidget : public QWidget
{
public:
    Q_OBJECT

public:
    explicit SelectVariableWidget(QWidget *parent = nullptr);
    ~SelectVariableWidget() override;

private:
    SelectVariableWidget(const SelectVariableWidget &) = delete;            // copy ctor
    SelectVariableWidget(SelectVariableWidget &&)      = delete;            // move ctor
    SelectVariableWidget &operator=(const SelectVariableWidget &) = delete; // copy assignment
    SelectVariableWidget &operator=(SelectVariableWidget &&) = delete;      // move assignment

private:
    Ui::SelectVariableWidget *ui{nullptr};
};

} // namespace preferences

#endif //GPUI_SELECT_VARIABLE_WIDGET_H
