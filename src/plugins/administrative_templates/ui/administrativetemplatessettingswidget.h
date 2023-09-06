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

#ifndef ADMINISTRATIVE_TEMPLATES_SETTINGS_WIDGET_H
#define ADMINISTRATIVE_TEMPLATES_SETTINGS_WIDGET_H

#include "../../core/isettingswidget.h"
#include "../../core/isnapinmanagementsettings.h"

#include "administrativetemplatessettingsobject.h"

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui
{
class AdministrativeTemplatesSettingsWidget;
}
QT_END_NAMESPACE

namespace gpui
{
class AdministrativeTemplatesSettingsWidget : public ISettingsWidget
{
public:
    Q_OBJECT

public:
    explicit AdministrativeTemplatesSettingsWidget(ISnapInManagementSettings *manager, QWidget *parent = nullptr);
    ~AdministrativeTemplatesSettingsWidget() override;

    QString getName() const override;

    void saveSettings() override;

private:
    AdministrativeTemplatesSettingsWidget(const AdministrativeTemplatesSettingsWidget &) = delete; // copy ctor
    AdministrativeTemplatesSettingsWidget(AdministrativeTemplatesSettingsWidget &&)      = delete; // move ctor
    AdministrativeTemplatesSettingsWidget &operator=(const AdministrativeTemplatesSettingsWidget &)
        = delete; // copy assignment
    AdministrativeTemplatesSettingsWidget &operator=(AdministrativeTemplatesSettingsWidget &&) = delete; // move assignment

private:
    Ui::AdministrativeTemplatesSettingsWidget *ui{nullptr};

    AdministrativeTemplatesSettingsObject settings;
};

} // namespace gpui

#endif //ADMINISTRATIVE_TEMPLATES_SETTINGS_WIDGET_H
