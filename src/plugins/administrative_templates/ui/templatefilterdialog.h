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

#include <QtWidgets>

namespace gpui
{
class TemplateFilterDialogPrivate;
class TemplateFilter;
class PlatformModel;

class TemplateFilterDialog final : public QDialog
{
    Q_OBJECT

public:
    TemplateFilterDialog(QWidget *parent = 0);
    ~TemplateFilterDialog();

    TemplateFilter getFilter() const;
    void setPlatformModel(PlatformModel *platformModel);
    void onLanguageChanged();

public slots:
    void open() override;
    void accept() override;
    void reject() override;

private:
    TemplateFilterDialogPrivate *const d;

private slots:
    void on_selectPushButton_clicked();
    void on_clearPushButton_clicked();

private:
    void setAllCheckState(Qt::CheckState);
    void clear();

    TemplateFilterDialog(const TemplateFilterDialog &) = delete;            // copy ctor
    TemplateFilterDialog(TemplateFilterDialog &&)      = delete;            // move ctor
    TemplateFilterDialog &operator=(const TemplateFilterDialog &) = delete; // copy assignment
    TemplateFilterDialog &operator=(TemplateFilterDialog &&) = delete;      // move assignment
};
} // namespace gpui

#endif // GPUI_TEMPLATEFILTERDIALOG_H
