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

#ifndef _SNAPINDETAILSDIALOG_H
#define _SNAPINDETAILSDIALOG_H

#include "core.h"

#include "isnapin.h"
#include "isnapindetailsdialog.h"

namespace Ui
{
class SnapInDetailsDialog;
}

namespace gpui
{
/**
 * @brief class SnapInDetailsDialog Shows properties of a snap-in.
 * Provides information on snap-in name, description, version, copyright, lisense.
 */
class GPUI_CORE_EXPORT SnapInDetailsDialog : public ISnapInDetailsDialog
{
private:
    Q_OBJECT

public:
    SnapInDetailsDialog(QWidget *parent, ISnapIn *snapIn);
    SnapInDetailsDialog(QWidget *parent = nullptr);
    ~SnapInDetailsDialog();

    void setSnapIn(ISnapIn *snapIn) override;

private:
    SnapInDetailsDialog(const SnapInDetailsDialog &) = delete;            // copy ctor
    SnapInDetailsDialog(SnapInDetailsDialog &&)      = delete;            // move ctor
    SnapInDetailsDialog &operator=(const SnapInDetailsDialog &) = delete; // copy assignment
    SnapInDetailsDialog &operator=(SnapInDetailsDialog &&) = delete;      // move assignment

private:
    Ui::SnapInDetailsDialog *ui;
};

} // namespace gpui

#endif //_SNAPINDETAILSDIALOG_H
