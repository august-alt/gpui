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

#ifndef GPUI_ADMINISTRATIVE_TEMPLATES_WIDGET_H
#define GPUI_ADMINISTRATIVE_TEMPLATES_WIDGET_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui
{
class AdministrativeTemplatesWidget;
}
QT_END_NAMESPACE

class QDataWidgetMapper;

namespace model
{
namespace registry
{
class AbstractRegistrySource;
}
} // namespace model

namespace gpui
{
class AdministrativeTemplatesWidgetPrivate;

class AdministrativeTemplatesWidget : public QWidget
{
    Q_OBJECT
public:
    enum PolicyWidgetState
    {
        STATE_NOT_CONFIGURED = 0,
        STATE_ENABLED        = 1,
        STATE_DISABLED       = 2
    };

public:
    explicit AdministrativeTemplatesWidget(QWidget *parent = nullptr);
    ~AdministrativeTemplatesWidget();

    void setModelIndex(const QModelIndex &index);

    void setUserRegistrySource(model::registry::AbstractRegistrySource *registrySource);
    void setMachineRegistrySource(model::registry::AbstractRegistrySource *registrySource);

signals:
    void savePolicyChanges();

private:
    AdministrativeTemplatesWidget(const AdministrativeTemplatesWidget &) = delete;            // copy ctor
    AdministrativeTemplatesWidget(AdministrativeTemplatesWidget &&)      = delete;            // move ctor
    AdministrativeTemplatesWidget &operator=(const AdministrativeTemplatesWidget &) = delete; // copy assignment
    AdministrativeTemplatesWidget &operator=(AdministrativeTemplatesWidget &&) = delete;      // move assignment

private:
    Ui::AdministrativeTemplatesWidget *ui{nullptr};

private:
    AdministrativeTemplatesWidgetPrivate *d;

private:
    void setPolicyWidgetState(AdministrativeTemplatesWidget::PolicyWidgetState state);

    void connectDialogBoxSignals();
    void onDataChanged();

    void onApplyClicked();
    void onCancelClicked();
};

} // namespace gpui

#endif //GPUI_ADMINISTRATIVE_TEMPLATES_WIDGET_H
