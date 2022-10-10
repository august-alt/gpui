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

#ifndef GPUI_PREFERENCES_TREE_PROXY_MODEL_H
#define GPUI_PREFERENCES_TREE_PROXY_MODEL_H

#include <QIdentityProxyModel>

#include <map>
#include <memory>

#include <string>

namespace preferences
{
class PreferencesModel;
class PreferencesTreeProxyModelPrivate;

class PreferencesTreeProxyModel : public QIdentityProxyModel
{
public:
    PreferencesTreeProxyModel();
    ~PreferencesTreeProxyModel();

    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const override;

    void setPreferencesModels(std::map<std::string, std::unique_ptr<PreferencesModel>> *machineModels,
                              std::map<std::string, std::unique_ptr<PreferencesModel>> *userModels);

private:
    PreferencesTreeProxyModelPrivate *d = nullptr;

private:
    PreferencesTreeProxyModel(const PreferencesTreeProxyModel &) = delete;            // copy ctor
    PreferencesTreeProxyModel(PreferencesTreeProxyModel &&)      = delete;            // move ctor
    PreferencesTreeProxyModel &operator=(const PreferencesTreeProxyModel &) = delete; // copy assignment
    PreferencesTreeProxyModel &operator=(PreferencesTreeProxyModel &&) = delete;      // move assignment
};
} // namespace preferences

#endif // GPUI_PREFERENCES_TREE_PROXY_MODEL_H
