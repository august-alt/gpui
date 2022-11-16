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

#ifndef SCRIPTWIDGETCOMMONSLOTS_H
#define SCRIPTWIDGETCOMMONSLOTS_H

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/viewmodel/viewmodel.h>
#include <qheaderview.h>

#include "basescriptswidget.h"
#include "groupscriptcontaineritem.h"
#include "scriptitemcontainer.h"

namespace scripts_plugin
{
class ScriptWidgetCommonSlots
{
public:
    ScriptWidgetCommonSlots(QWidget *p, BaseScriptWidget *base);
    ~ScriptWidgetCommonSlots() = default;

    ScriptWidgetCommonSlots(const ScriptWidgetCommonSlots &) = delete;            // copy ctor
    ScriptWidgetCommonSlots(ScriptWidgetCommonSlots &&)      = delete;            // move ctor
    ScriptWidgetCommonSlots &operator=(const ScriptWidgetCommonSlots &) = delete; // copy assignment
    ScriptWidgetCommonSlots &operator=(ScriptWidgetCommonSlots &&) = delete;      // move assignment

    /**
    * @brief onUpClicked Moves selected element up the list.
    */
    void onUpClicked();

    /**
    * @brief onDownClicked Moves selected element down the list.
    */
    void onDownClicked();

    /**
    * @brief onAddClicked Launches widget to add new element to the list.
    */
    void onAddClicked();

    /**
    * @brief onEditClicked Launches widget to edit selected element.
    */
    void onEditClicked();

    /**
    * @brief onDeleteClicked Removes selected element.
    */
    void onDeleteClicked();

    /**
     * @brief onBrowseClicked Open ini file
     */
    void onBrowseClicked();

    /**
     * @brief onOkClicked saves ini file
     */
    void onOkClicked();

private:
    QWidget *parent;

    BaseScriptWidget *m_base;

private:
    void loadIniFile(QString file);
};

} // namespace scripts_plugin

#endif // SCRIPTWIDGETCOMMONSLOTS_H
