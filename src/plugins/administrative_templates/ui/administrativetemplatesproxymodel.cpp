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

#include "administrativetemplatesproxymodel.h"

#include "../bundle/itemtype.h"
#include "../bundle/policyroles.h"

#include "administrativetemplateswidget.h"

namespace gpui
{
class AdministrativeTemplatesProxyModelPrivate
{
public:
    model::registry::AbstractRegistrySource *userSource    = nullptr;
    model::registry::AbstractRegistrySource *machineSource = nullptr;
};

AdministrativeTemplatesProxyModel::AdministrativeTemplatesProxyModel()
    : QIdentityProxyModel()
    , d(new AdministrativeTemplatesProxyModelPrivate())
{}

AdministrativeTemplatesProxyModel::~AdministrativeTemplatesProxyModel()
{
    delete d;
}

QVariant AdministrativeTemplatesProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    if (role == model::bundle::POLICY_WIDGET)
    {
        std::function<QWidget *()> widgetCreator = [=]() {
            auto contentWidget = new AdministrativeTemplatesWidget();

            contentWidget->setMachineRegistrySource(d->machineSource);
            contentWidget->setUserRegistrySource(d->userSource);

            contentWidget->setModelIndex(proxyIndex);

            connect(contentWidget,
                    &AdministrativeTemplatesWidget::savePolicyChanges,
                    this,
                    &AdministrativeTemplatesProxyModel::savePolicyChanges);
            return contentWidget;
        };

        return QVariant::fromValue(widgetCreator);
    }

    return QIdentityProxyModel::data(proxyIndex, role);
}

void AdministrativeTemplatesProxyModel::setUserRegistrySource(model::registry::AbstractRegistrySource *registrySource)
{
    d->userSource = registrySource;
}

void AdministrativeTemplatesProxyModel::setMachineRegistrySource(model::registry::AbstractRegistrySource *registrySource)
{
    d->machineSource = registrySource;
}

} // namespace gpui

Q_DECLARE_METATYPE(std::function<QWidget *()>)
