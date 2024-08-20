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

#ifndef MVVM_FOLDERS_SCRIPTS_WIDGET_H
#define MVVM_FOLDERS_SCRIPTS_WIDGET_H

#include "basescripttabwidget.h"

#include <mvvm/viewmodel/viewmodeldelegate.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
class ScriptsWidget;
}
QT_END_NAMESPACE

namespace scripts_plugin
{
class GroupScriptContainerItem;

class ScriptsWidget : public QWidget, public BaseScriptTabWidget
{
public:
    Q_OBJECT
public:
    explicit ScriptsWidget(QWidget *parent = nullptr);
    ~ScriptsWidget();

    void setItem(ScriptItemContainer *item, bool isStartUpFlag, bool isUserFlag);

private slots:
    void on_upPushButton_clicked();

    void on_downPushButton_clicked();

    void on_addPushButton_clicked();

    void on_editPushButton_clicked();

    void on_removePushButton_clicked();

    void on_showPushButton_clicked();

private:
    ScriptsWidget(const ScriptsWidget &) = delete;            // copy ctor
    ScriptsWidget(ScriptsWidget &&)      = delete;            // move ctor
    ScriptsWidget &operator=(const ScriptsWidget &) = delete; // copy assignment
    ScriptsWidget &operator=(ScriptsWidget &&) = delete;      // move assignment

private:
    Ui::ScriptsWidget *ui = nullptr;
};

} // namespace scripts_plugin

#endif //MVVM_FOLDERS_SCRIPTS_WIDGET_H
