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

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include "widgetfactory.h"

#include <mvvm/model/sessionmodel.h>

namespace preferences
{
PreferencesDialog::PreferencesDialog(ModelView::SessionItem *item, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PreferencesDialog())
{
    ui->setupUi(this);

    auto factory = std::make_unique<WidgetFactory>();

    if (item)
    {
        ui->commonTab->setItem(item->children()[item->children().size() - 2]);

        auto widgets = factory->create(item->children().back()->modelType());
        for (auto &widget : widgets)
        {
            widget->setItem(item->children().back());
            connect(ui->buttonBox, &QDialogButtonBox::accepted, widget.get(), &PreferenceWidgetInterface::submit);

            // TODO: Replace with proper condition checking.
            // For multiple widgets this will not work.
            // Check if signal dataChanged has fired for all
            // of content widgets.
            connect(widget.get(), &PreferenceWidgetInterface::dataChanged, this, &QDialog::accept);

            auto widgetName = widget->name();

            ui->tabWidget->insertTab(0, widget.release(), widgetName);
        }
        ui->tabWidget->setCurrentIndex(0);
    }

    connect(ui->buttonBox, &QDialogButtonBox::accepted, ui->commonTab, &CommonView::submit);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

} // namespace preferences
