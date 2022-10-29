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

#include "administrativetemplateswidget.h"
#include "ui_administrativetemplateswidget.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

#include "../registry/abstractregistrysource.h"
#include "../registry/policystatemanager.h"

namespace gpui
{
class AdministrativeTemplatesWidgetPrivate
{
public:
    model::registry::AbstractRegistrySource *userSource    = nullptr;
    model::registry::AbstractRegistrySource *machineSource = nullptr;

    std::unique_ptr<model::registry::PolicyStateManager> manager = nullptr;

    bool dataChanged  = false;
    bool stateEnabled = false;

    QModelIndex currentIndex{};

    AdministrativeTemplatesWidget::PolicyWidgetState state
        = AdministrativeTemplatesWidget::PolicyWidgetState::STATE_NOT_CONFIGURED;
    AdministrativeTemplatesWidget::PolicyWidgetState initialState
        = AdministrativeTemplatesWidget::PolicyWidgetState::STATE_NOT_CONFIGURED;
};

AdministrativeTemplatesWidget::AdministrativeTemplatesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdministrativeTemplatesWidget())
    , d(new AdministrativeTemplatesWidgetPrivate())
{
    ui->setupUi(this);

    connect(ui->notConfiguredRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            d->dataChanged = d->state != STATE_NOT_CONFIGURED && d->initialState != STATE_NOT_CONFIGURED;
            setPolicyWidgetState(STATE_NOT_CONFIGURED);
            qWarning() << "Setting state not configured" << d->manager.get();
            if (d->manager)
            {
                d->manager->setupPolicyState(model::registry::PolicyStateManager::STATE_NOT_CONFIGURED);
            }
        }
    });
    connect(ui->enabledRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            d->dataChanged = d->state != STATE_ENABLED && d->initialState != STATE_ENABLED;
            setPolicyWidgetState(STATE_ENABLED);
            qWarning() << "Setting state enabled" << d->manager.get();
            if (d->manager)
            {
                d->manager->setupPolicyState(model::registry::PolicyStateManager::STATE_ENABLED);
            }
        }
    });
    connect(ui->disabledRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            d->dataChanged = d->state != STATE_DISABLED && d->initialState != STATE_DISABLED;
            setPolicyWidgetState(STATE_DISABLED);
            qWarning() << "Setting state disabled" << d->manager.get();
            if (d->manager)
            {
                d->manager->setupPolicyState(model::registry::PolicyStateManager::STATE_DISABLED);
            }
        }
    });
}

AdministrativeTemplatesWidget::~AdministrativeTemplatesWidget()
{
    delete ui;
}

void AdministrativeTemplatesWidget::setPolicyWidgetState(AdministrativeTemplatesWidget::PolicyWidgetState state)
{
    switch (state)
    {
    case STATE_ENABLED: {
        d->stateEnabled = true;
        ui->contentScrollArea->setDisabled(false);
    }
    break;
    case STATE_DISABLED:
    case STATE_NOT_CONFIGURED:
    default:
        d->stateEnabled = false;
        ui->contentScrollArea->setDisabled(true);
        break;
    }

    d->state = state;
}

} // namespace gpui
