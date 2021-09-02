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

#include "model.h"
#include "registry/policystatemanager.h"

#include <QSortFilterProxyModel>

namespace model {
    namespace registry {
        class AbstractRegistrySource;
    }
}

namespace model {

class TemplateFilterModelPrivate;

/*!
 * \class TemplateFilterModel
 * \brief The TemplateFilterModel class
 *
 * \ingroup model
 */
class GPUI_MODEL_EXPORT TemplateFilterModel final : public QSortFilterProxyModel
{
public:
    TemplateFilterModel(QObject *parent);
    ~TemplateFilterModel();

    void setTitleFilter(const QString &title_filter);
    void setTitleFilterEnabled(const bool enabled);
    void setStateFilter(const model::registry::PolicyStateManager::PolicyState state_filter);
    void setStateFilterEnabled(const bool enabled);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    void setUserRegistrySource(model::registry::AbstractRegistrySource* registrySource);
    void setMachineRegistrySource(model::registry::AbstractRegistrySource* registrySource);

private:
    TemplateFilterModelPrivate* d;
};

}

#endif // GPUI_TEMPLATEFILTERMODEL_H
