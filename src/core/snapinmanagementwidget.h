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

#ifndef _SNAPINMANAGEMENTWIDGET_H
#define _SNAPINMANAGEMENTWIDGET_H

#include "core.h"

#include <QWidget>

#include "isnapinmanager.h"

namespace Ui
{
class SnapInManagementWidget;
}

class QTreeWidgetItem;

namespace gpui
{
class SnapInManagementWidgetPrivate;

/**
 * @brief The SnapInManagementWidget class Shows tree of available snap-ins. Enables and disables snap-ins.
 */
class GPUI_CORE_EXPORT SnapInManagementWidget : public QWidget
{
    Q_OBJECT

public:
    SnapInManagementWidget(QWidget *parent, ISnapInManager *manager);
    ~SnapInManagementWidget();

private slots:
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    SnapInManagementWidget(const SnapInManagementWidget &) = delete;            // copy ctor
    SnapInManagementWidget(SnapInManagementWidget &&)      = delete;            // move ctor
    SnapInManagementWidget &operator=(const SnapInManagementWidget &) = delete; // copy assignment
    SnapInManagementWidget &operator=(SnapInManagementWidget &&) = delete;      // move assignment

private:
    SnapInManagementWidgetPrivate *d;
};

} // namespace gpui

#endif //_SNAPINMANAGEMENTWIDGET_H
