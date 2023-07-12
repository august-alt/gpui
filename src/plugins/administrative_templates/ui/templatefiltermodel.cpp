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
#include "ui/platformmodel.h"

#include "admx/supporteddefinition.h"

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

    SupportedDefinitions supportedOnDefinitions{};

    PlatformModel *platformModel = nullptr;

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

void TemplateFilterModel::setSupportedOnDefenitions(const SupportedDefinitions &supportedOnDefinitions)
{
    d->supportedOnDefinitions = supportedOnDefinitions;
}

void TemplateFilterModel::setPlatformModel(PlatformModel *model)
{
    d->platformModel = model;
}

// NOTE: filterAcceptsRow() is split in 2 parts because the
// code to get policy state is too hard to mock so we avoid
// testing it.
bool TemplateFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto state              = PolicyStateManager::STATE_NOT_CONFIGURED;
    const auto policy       = index.model()->data(index, PolicyRoles::POLICY).value<PolicyPtr>();
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

    const bool configuredMatch = d->filter.configured.contains(state);

    return filterPlatform(index) && filterKeyword(index) && configuredMatch;
}

bool TemplateFilterModel::filterPlatform(const QModelIndex &platformIndex) const
{
    if (!d->filter.platformEnabled)
    {
        return true;
    }

    const QString &supportedOnFull = platformIndex.data(PolicyRoles::SUPPORTED_ON).value<QString>();
    if (supportedOnFull.isEmpty())
    {
        return false;
    }

    const std::string supportedOnReference           = supportedOnFull.split(':').last().toStdString();
    std::shared_ptr<SupportedDefinition> supportedOn = d->supportedOnDefinitions[supportedOnReference];

    // NOTE(mchernigin): supportedOn will be nullptr if current item is a folder. In this case it should be filtered out
    // only if all of it's children are filtered out. `if` bellow checks exactly that.
    //
    // For now, filterPlatform is called multiple times for all items which are contained in some
    // folder. Once from filterAcceptRow and on each filterPlatform call on its parent. There is room for optimization
    if (!supportedOn)
    {
        for (int i = 0; i < d->platformModel->rowCount(platformIndex); ++i)
        {
            if (filterPlatform(d->platformModel->index(i, 0, platformIndex)))
            {
                return true;
            }
        }

        return false;
    }

    const auto matchSinglePlatform = [&](QString platform) {
        const auto isPlatformWithinRange = [&](SupportedOnRange range) {
            QString parentReference = QString::fromStdString(range.itemReference).split(':').last();
            int version             = d->platformModel->getPlatformIndex(platform, parentReference);
            if (version == -1)
            {
                return false;
            }

            return (range.minVersionIndex <= static_cast<uint32_t>(version)
                    && static_cast<uint32_t>(version) <= range.maxVersionIndex);
        };

        if (!supportedOn->or_.empty())
        {
            return std::any_of(supportedOn->or_.begin(), supportedOn->or_.end(), isPlatformWithinRange);
        }
        if (!supportedOn->and_.empty())
        {
            return std::all_of(supportedOn->and_.begin(), supportedOn->and_.end(), isPlatformWithinRange);
        }

        return false;
    };

    const QSet<QString> &selectedPlatforms = d->filter.selectedPlatforms;
    switch (d->filter.platformType)
    {
    case PlatformFilterType_ANY:
        return std::any_of(selectedPlatforms.begin(), selectedPlatforms.end(), matchSinglePlatform);
    case PlatformFilterType_ALL:
        return std::all_of(selectedPlatforms.begin(), selectedPlatforms.end(), matchSinglePlatform);
    default:
        return false;
    }
}

bool TemplateFilterModel::filterKeyword(const QModelIndex &index) const
{
    if (!d->filter.keywordEnabled)
    {
        return true;
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
    return keywordMatch;
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
