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

#ifndef GPUI_TEMPLATEFILTERMODEL_H
#define GPUI_TEMPLATEFILTERMODEL_H

#include "registry/policystatemanager.h"

#include "admx/supporteddefinition.h"
#include "ui/platformmodel.h"

#include <string>
#include <QSortFilterProxyModel>

namespace model
{
namespace registry
{
class AbstractRegistrySource;
}
} // namespace model

namespace gpui
{
class TemplateFilterModelPrivate;
class TemplateFilter;

/*!
 * \class TemplateFilterModel
 * \brief The TemplateFilterModel class
 *
 * \ingroup gpui
 */
class TemplateFilterModel final : public QSortFilterProxyModel
{
public:
    TemplateFilterModel(QObject *parent = nullptr);
    ~TemplateFilterModel();

    void setFilter(const TemplateFilter &filter, const bool enabled);
    void setSupportedOnDefenitions(const model::admx::SupportedDefinitions &SupportedOnDefinitions);
    void setPlatformModel(PlatformModel *model);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    void setUserRegistrySource(model::registry::AbstractRegistrySource *registrySource);
    void setMachineRegistrySource(model::registry::AbstractRegistrySource *registrySource);

    bool filterAcceptsRow(const QModelIndex &index, const model::registry::PolicyStateManager::PolicyState state) const;

    void onLanguageChanged();

private:
    TemplateFilterModel(const TemplateFilterModel &) = delete;            // copy ctor
    TemplateFilterModel(TemplateFilterModel &&)      = delete;            // move ctor
    TemplateFilterModel &operator=(const TemplateFilterModel &) = delete; // copy assignment
    TemplateFilterModel &operator=(TemplateFilterModel &&) = delete;      // move assignment

    bool filterPlatform(const QModelIndex &index) const;
    bool filterKeyword(const QModelIndex &index) const;

private:
    TemplateFilterModelPrivate *d;
};

} // namespace gpui

#endif // GPUI_TEMPLATEFILTERMODEL_H
