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

#ifndef _COMPOSITESNAPINDETAILSDIALOG_H
#define _COMPOSITESNAPINDETAILSDIALOG_H

#include "core.h"

#include "isnapin.h"
#include "isnapindetailsdialog.h"

namespace Ui
{
class CompositeSnapInDetailsDialog;
}

namespace gpui
{
/**
 * @brief Shows properties and dependencies of composite snap-in.
 */
class GPUI_CORE_EXPORT CompositeSnapInDetailsDialog : public ISnapInDetailsDialog
{
    Q_OBJECT

public:
    CompositeSnapInDetailsDialog(QWidget *parent, ISnapIn *snapIn);
    CompositeSnapInDetailsDialog(QWidget *parent = nullptr);
    ~CompositeSnapInDetailsDialog();

    void setSnapIn(ISnapIn *snapIn) override;

private:
    CompositeSnapInDetailsDialog(const CompositeSnapInDetailsDialog &) = delete;            // copy ctor
    CompositeSnapInDetailsDialog(CompositeSnapInDetailsDialog &&)      = delete;            // move ctor
    CompositeSnapInDetailsDialog &operator=(const CompositeSnapInDetailsDialog &) = delete; // copy assignment
    CompositeSnapInDetailsDialog &operator=(CompositeSnapInDetailsDialog &&) = delete;      // move assignment

private:
    Ui::CompositeSnapInDetailsDialog *ui;
};

} // namespace gpui

#endif //_COMPOSITESNAPINDETAILSDIALOG_H
