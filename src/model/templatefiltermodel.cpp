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

#include "templatefiltermodel.h"

#include "bundle/policyroles.h"
#include "registry/abstractregistrysource.h"
#include "registry/policystatemanager.h"
#include "admx/policy.h"
#include "bundle/policyroles.h"
#include "templatefilter.h"

#include <QDebug>

namespace model {

typedef std::shared_ptr<admx::Policy> PolicyPtr;

class TemplateFilterModelPrivate
{
public:  
    // TODO: don't like these members, they are already in
    // main window
    registry::AbstractRegistrySource* userSource = nullptr;
    registry::AbstractRegistrySource* machineSource = nullptr;

    TemplateFilter filter;
};

TemplateFilterModel::TemplateFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , d(new TemplateFilterModelPrivate())
{
    d->filter.titleFilter = QString();
    d->filter.titleFilterEnabled = false;
    d->filter.stateFilter = registry::PolicyStateManager::STATE_NOT_CONFIGURED;
    d->filter.stateFilterEnabled = false;

    setRecursiveFilteringEnabled(true);
}

TemplateFilterModel::~TemplateFilterModel()
{
    delete d;
}

void TemplateFilterModel::setFilter(const TemplateFilter &filter)
{
    d->filter = filter;

    invalidateFilter();
}

bool TemplateFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    // TODO: remove magic number "1"
    const bool itemIsTemplate = [&]()
    {
        const uint item_type = index.data(bundle::PolicyRoles::ITEM_TYPE).value<uint>();
        const bool out = (item_type == 1);

        return out;
    }();

    const bool titleFilterMatch = [&]()
    {
        const QString text = index.data().value<QString>();
        const bool out = (text.contains(d->filter.titleFilter));

        return out;
    }();

    // TODO: this is very convoluted, also duplicating
    // part of ContentWidget::onListItemClicked()
    const bool stateFilterMatch = [&]()
    {
        const QAbstractItemModel* model = index.model();
        auto policy = model->data(index, bundle::PolicyRoles::POLICY).value<PolicyPtr>();

        if (policy == nullptr) {
            return false;
        }

        const auto source = [&]() {
            if (policy->policyType == admx::PolicyType::Machine)
            {
                return d->machineSource;
            } else {
                return d->userSource;
            }
        }();

        if (source == nullptr) {
            return false;
        }

        const auto manager = std::make_unique<registry::PolicyStateManager>(*source, *policy);

        auto state = manager->determinePolicyState();
        const bool out = (state == registry::PolicyStateManager::STATE_NOT_CONFIGURED);

        return out;
    }();

    // Don't filter non-template types, so that the rest of
    // the tree appears fully
    if (!itemIsTemplate)
    {
        return true;
    }

    if (d->filter.titleFilterEnabled && !titleFilterMatch)
    {
        return false;
    }

    if (d->filter.stateFilterEnabled && !stateFilterMatch)
    {
        return false;
    }

    return true;
}

void TemplateFilterModel::setUserRegistrySource(registry::AbstractRegistrySource *registrySource)
{
    d->userSource = registrySource;
}

void TemplateFilterModel::setMachineRegistrySource(registry::AbstractRegistrySource *registrySource)
{
    d->machineSource = registrySource;
}

}

Q_DECLARE_METATYPE(std::shared_ptr<model::admx::Policy>)
