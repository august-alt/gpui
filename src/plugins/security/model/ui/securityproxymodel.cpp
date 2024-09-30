/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#include "securityproxymodel.h"

#include "../administrative_templates/bundle/itemtype.h"
#include "../administrative_templates/bundle/policyroles.h"

#include "securitywidget.h"

namespace security
{

class SecurityProxyModelPrivate
{
public:
};

SecurityProxyModel::SecurityProxyModel()
    : d(new SecurityProxyModelPrivate())
{
}

SecurityProxyModel::~SecurityProxyModel()
{
    delete d;
}

QVariant SecurityProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    if (role == model::bundle::POLICY_WIDGET)
    {
        std::function<QWidget *()> widgetCreator = [=]() {
            auto contentWidget = new SecurityWidget();

            contentWidget->setMachineSecuritySource();
            contentWidget->setUserSecuritySource();

            contentWidget->setModelIndex(proxyIndex);

            connect(contentWidget,
                    &SecurityWidget::savePolicyChanges,
                    this,
                    &SecurityProxyModel::savePolicyChanges);
            return contentWidget;
        };

        return QVariant::fromValue(widgetCreator);
    }

    return QIdentityProxyModel::data(proxyIndex, role);
}

} // namespace security

Q_DECLARE_METATYPE(std::function<QWidget *()>)
