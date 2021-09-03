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

#ifndef GPUI_TEMPLATEFILTERDIALOG_H
#define GPUI_TEMPLATEFILTERDIALOG_H

#include "gui.h"

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class TemplateFilterDialog; }
QT_END_NAMESPACE

namespace model {
    class TemplateFilter;
}

namespace gpui {

    class TemplateFilterDialogPrivate;

    class GPUI_GUI_EXPORT TemplateFilterDialog final : public QDialog {
        Q_OBJECT

    public:
        TemplateFilterDialog(QWidget *parent = 0);
        ~TemplateFilterDialog();

        model::TemplateFilter getFilter() const;

    public slots:
        void accept() override;

    private:

        TemplateFilterDialogPrivate* const d;

    private slots:

    private:

    };
}

#endif // GPUI_TEMPLATEFILTERDIALOG_H
