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

#ifndef _SNAPINDETAILSWIDGET_H
#define _SNAPINDETAILSWIDGET_H

#include "core.h"

#include <QWidget>

#include "isnapin.h"

namespace Ui
{
class SnapInDetailsWidget;
}

namespace gpui
{
/**
 * @brief class SnapInDetailsWidget Shows properties of a snap-in.
 * Provides information on snap-in name, description, version, copyright, lisense.
 */
class GPUI_CORE_EXPORT SnapInDetailsWidget : public QWidget
{
private:
    Q_OBJECT

public:
    SnapInDetailsWidget(QWidget *parent, ISnapIn *snapIn);
    SnapInDetailsWidget(QWidget *parent);
    ~SnapInDetailsWidget();

    void setSnapIn(const ISnapIn *snapIn);

private:
    SnapInDetailsWidget(const SnapInDetailsWidget &) = delete;            // copy ctor
    SnapInDetailsWidget(SnapInDetailsWidget &&)      = delete;            // move ctor
    SnapInDetailsWidget &operator=(const SnapInDetailsWidget &) = delete; // copy assignment
    SnapInDetailsWidget &operator=(SnapInDetailsWidget &&) = delete;      // move assignment

private:
    Ui::SnapInDetailsWidget *ui;
};

} // namespace gpui

#endif //_SNAPINDETAILSWIDGET_H
