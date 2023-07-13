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

#ifndef GPUI_ADMINISTRATIVE_TEMPLATES_PROXY_MODEL_H
#define GPUI_ADMINISTRATIVE_TEMPLATES_PROXY_MODEL_H

#include <QIdentityProxyModel>

namespace comments
{
class CommentsModel;
}

namespace model
{
namespace registry
{
class AbstractRegistrySource;
}
} // namespace model

namespace gpui
{
class AdministrativeTemplatesProxyModelPrivate;

class AdministrativeTemplatesProxyModel : public QIdentityProxyModel
{
    Q_OBJECT

public:
    AdministrativeTemplatesProxyModel();
    ~AdministrativeTemplatesProxyModel();

    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const override;

    void setUserRegistrySource(model::registry::AbstractRegistrySource *registrySource);
    void setMachineRegistrySource(model::registry::AbstractRegistrySource *registrySource);

    void setUserCommentModel(comments::CommentsModel* userCommentModel);
    void setMachineCommentModel(comments::CommentsModel* machineCommentModel);

signals:
    void savePolicyChanges();

private:
    AdministrativeTemplatesProxyModelPrivate *d = nullptr;

private:
    AdministrativeTemplatesProxyModel(const AdministrativeTemplatesProxyModel &) = delete;            // copy ctor
    AdministrativeTemplatesProxyModel(AdministrativeTemplatesProxyModel &&)      = delete;            // move ctor
    AdministrativeTemplatesProxyModel &operator=(const AdministrativeTemplatesProxyModel &) = delete; // copy assignment
    AdministrativeTemplatesProxyModel &operator=(AdministrativeTemplatesProxyModel &&) = delete;      // move assignment
};
} // namespace gpui

#endif // GPUI_ADMINISTRATIVE_TEMPLATES_PROXY_MODEL_H
