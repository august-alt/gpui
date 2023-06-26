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

#include "templatefilter.h"

#include "../plugins/administrative_templates/admx/policy.h"
#include "../plugins/administrative_templates/admx/policyelement.h"
#include "../plugins/administrative_templates/bundle/policyroles.h"
#include "../plugins/administrative_templates/registry/abstractregistrysource.h"
#include "../plugins/administrative_templates/registry/policystatemanager.h"

#include <QDebug>

using namespace model::registry;
using namespace model::admx;
using namespace model::bundle;

namespace gpui
{
typedef std::shared_ptr<Policy> PolicyPtr;

class TemplateFilterModelPrivate
{
public:
    // TODO: don't like these members, they are already in
    // main window
    AbstractRegistrySource *userSource    = nullptr;
    AbstractRegistrySource *machineSource = nullptr;

    TemplateFilter filter{};
    bool enabled = false;
};

TemplateFilterModel::TemplateFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , d(new TemplateFilterModelPrivate())
{
    d->filter.keywordEnabled = false;
    d->filter.titleEnabled   = false;
    d->filter.helpEnabled    = false;
    d->filter.commentEnabled = false;
    d->filter.keywordType    = KeywordFilterType_ANY;
    d->filter.keywordText    = QString();
    ;

    d->filter.configured = QSet<PolicyStateManager::PolicyState>();

    d->enabled = false;

    setRecursiveFilteringEnabled(true);
}

TemplateFilterModel::~TemplateFilterModel()
{
    delete d;
}

void TemplateFilterModel::setFilter(const TemplateFilter &filter, const bool enabled)
{
    d->filter  = filter;
    d->enabled = enabled;

    invalidateFilter();
}

// NOTE: filterAcceptsRow() is split in 2 parts because the
// code to get policy state is too hard to mock so we avoid
// testing it.
bool TemplateFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    const auto policy       = index.model()->data(index, PolicyRoles::POLICY).value<PolicyPtr>();
    auto state              = PolicyStateManager::STATE_NOT_CONFIGURED;
    if (policy != nullptr)
    {
        const auto source = (policy->policyType == PolicyType::Machine ? d->machineSource : d->userSource);
        if (source != nullptr)
        {
            const auto manager = std::make_unique<PolicyStateManager>(*source, *policy);
            state              = manager->determinePolicyState();
        }
    }

    return filterAcceptsRow(index, state);
}

bool TemplateFilterModel::filterAcceptsRow(const QModelIndex &index, const PolicyStateManager::PolicyState state) const
{
    if (!d->enabled)
    {
        return true;
    }

    const auto supportedPlatforms    = index.data(PolicyRoles::SUPPORTED_ON).value<QString>();
    const QSet<QString> platformList = d->filter.selectedPlatforms;
    bool platformMatch               = false;
    switch (d->filter.platformType)
    {
    case PlatformFilterType_ANY:
        platformMatch = std::any_of(platformList.begin(), platformList.end(), [&supportedPlatforms](QString platform) {
            return supportedPlatforms.contains(platform);
        });
        break;
    case PlatformFilterType_ALL:
        platformMatch = std::all_of(platformList.begin(), platformList.end(), [&supportedPlatforms](QString platform) {
            return supportedPlatforms.contains(platform);
        });
        break;
    }

    auto checkKeywordMatch = [&](const QString &string) {
        const QList<QString> keywordList = d->filter.keywordText.split(" ");

        switch (d->filter.keywordType)
        {
        case KeywordFilterType_ANY:
            return std::any_of(keywordList.begin(), keywordList.end(), [&string](QString keyword) {
                return string.contains(keyword);
            });
        case KeywordFilterType_ALL:
            return std::all_of(keywordList.begin(), keywordList.end(), [&string](QString keyword) {
                return string.contains(keyword);
            });
        case KeywordFilterType_EXACT:
            return string.contains(d->filter.keywordText);
        default:
            return false;
        }
    };

    const QString titleText = index.data(Qt::DisplayRole).value<QString>();
    const bool titleMatch   = checkKeywordMatch(titleText);

    const QString helpText = index.data(PolicyRoles::EXPLAIN_TEXT).value<QString>();
    const bool helpMatch   = checkKeywordMatch(helpText);

    // TODO: implement comment filter (comment data not stored in model yet)
    const bool commentMatch = true;

    const bool keywordMatch = (d->filter.titleEnabled && titleMatch) || (d->filter.helpEnabled && helpMatch)
                              || (d->filter.commentEnabled && commentMatch);

    const bool configuredMatch = d->filter.configured.contains(state);

    return (!d->filter.platformEnabled || platformMatch) && (!d->filter.keywordEnabled || keywordMatch)
           && configuredMatch;
}

void TemplateFilterModel::setUserRegistrySource(AbstractRegistrySource *registrySource)
{
    d->userSource = registrySource;
}

void TemplateFilterModel::setMachineRegistrySource(AbstractRegistrySource *registrySource)
{
    d->machineSource = registrySource;
}

} // namespace gpui

Q_DECLARE_METATYPE(std::shared_ptr<Policy>)
