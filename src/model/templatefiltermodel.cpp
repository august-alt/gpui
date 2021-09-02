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

    QString title_filter;
    bool title_filter_enabled;
    registry::PolicyStateManager::PolicyState state_filter;
    bool state_filter_enabled;
};

TemplateFilterModel::TemplateFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , d(new TemplateFilterModelPrivate())
{
    d->title_filter = QString();
    d->title_filter_enabled = false;
    d->state_filter = registry::PolicyStateManager::STATE_NOT_CONFIGURED;
    d->state_filter_enabled = false;

    setRecursiveFilteringEnabled(true);
}

TemplateFilterModel::~TemplateFilterModel()
{
    delete d;
}

void TemplateFilterModel::set_title_filter(const QString &title_filter)
{
    d->title_filter = title_filter;
}

void TemplateFilterModel::set_title_filter_enabled(const bool enabled)
{
    d->title_filter_enabled = enabled;
}

void TemplateFilterModel::set_state_filter(const registry::PolicyStateManager::PolicyState state_filter)
{
    d->state_filter = state_filter;
}

void TemplateFilterModel::set_state_filter_enabled(const bool enabled)
{
    d->state_filter_enabled = enabled;
}

bool TemplateFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    // TODO: remove magic number "1"
    const bool item_is_template = [&]()
    {
        const uint item_type = index.data(bundle::PolicyRoles::ITEM_TYPE).value<uint>();
        const bool out = (item_type == 1);

        return out;
    }();

    const bool title_filter_match = [&]()
    {
        const QString text = index.data().value<QString>();
        const bool out = (text.contains(d->title_filter));

        return out;
    }();

    // TODO: this is very convoluted, also duplicating
    // part of ContentWidget::onListItemClicked()
    const bool state_filter_match = [&]()
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
    if (!item_is_template)
    {
        return true;
    }

    if (d->title_filter_enabled && !title_filter_match)
    {
        return false;
    }

    if (d->state_filter_enabled && !state_filter_match)
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
