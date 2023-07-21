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

#ifndef GPUI_PLATFORM_MODEL_H
#define GPUI_PLATFORM_MODEL_H

#include <QStandardItemModel>

#include "../admx/supportedproduct.h"

#include <memory>

namespace gpui
{
enum PlatformRole
{
    PLATFORM_ROLE_SORT = Qt::UserRole + 1,
};

class PlatformModelPrivate;

/*!
 * \class PlatformModel
 * \brief The PlatformModel class
 *
 * \ingroup gpui
 */
class PlatformModel final : public QStandardItemModel
{
public:
    PlatformModel();
    ~PlatformModel();

    void populateModel(std::vector<std::shared_ptr<model::admx::SupportedProduct>> products);
    void buildPlatformMap();
    int getPlatformIndex(QString platform, QString parentReference) const;

private:
    PlatformModel(const PlatformModel &) = delete;            // copy ctor
    PlatformModel(PlatformModel &&)      = delete;            // move ctor
    PlatformModel &operator=(const PlatformModel &) = delete; // copy assignment
    PlatformModel &operator=(PlatformModel &&) = delete;      // move assignment

private:
    PlatformModelPrivate *d;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::CheckState getItemCheckStateBasedOnChildren(const QStandardItem *parent) const;
};

} // namespace gpui

#endif // GPUI_PLATFORM_MODEL_H
