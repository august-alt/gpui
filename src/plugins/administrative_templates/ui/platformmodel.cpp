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

#include "platformmodel.h"
#include "qnamespace.h"

#include <memory>

#include <QDebug>
#include <QSet>
#include <QString>

namespace gpui
{
class PlatformModelPrivate
{
public:
    QStandardItemModel *sourceModel = nullptr;
    std::vector<std::shared_ptr<model::admx::SupportedProduct>> items{};

    PlatformModelPrivate() {}
    ~PlatformModelPrivate() {}

    PlatformModelPrivate(const PlatformModelPrivate &) = delete;            // copy ctor
    PlatformModelPrivate(PlatformModelPrivate &&)      = delete;            // move ctor
    PlatformModelPrivate &operator=(const PlatformModelPrivate &) = delete; // copy assignment
    PlatformModelPrivate &operator=(PlatformModelPrivate &&) = delete;      // move assignment
};

class PlatformItem : public QStandardItem
{
public:
    PlatformItem(const QString &name);
};

PlatformItem::PlatformItem(const QString &name)
{
    setText(name);
    setEditable(false);
    setCheckable(true);
}

PlatformModel::PlatformModel()
    : QStandardItemModel()
    , d(new PlatformModelPrivate())
{}

PlatformModel::~PlatformModel()
{
    delete d;
}

// NOTE: allows recursively select children using checkbox
bool PlatformModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    auto item = itemFromIndex(index);

    // (un)check all children on item (un)check
    for (int i = 0; i < item->rowCount(); i++)
    {
        QStandardItem *childItem = item->child(i);
        setData(childItem->index(), value, role);
    }

    // uncheck all parent if item was unchecked
    bool isItemBeingUnchecked = (item->checkState() == Qt::CheckState::Checked); // effect was not applied yet
    if (!isItemBeingUnchecked)
    {
        for (auto parent = item->parent(); parent; parent = parent->parent())
        {
            QStandardItemModel::setData(parent->index(), value, role);
        }
    }

    return QStandardItemModel::setData(index, value, role);
}

void PlatformModel::populateModel(std::vector<std::shared_ptr<model::admx::SupportedProduct>> products)
{
    clear();

    d->items = products;

    for (const auto &product : d->items)
    {
        auto productElement = new PlatformItem(QString::fromStdString(product->displayName));

        int majorVersionIdx = 0;
        for (const auto &majorVersion : product->majorVersion)
        {
            auto majorVersionElement = new PlatformItem(QString::fromStdString(majorVersion.displayName));

            int minorVersionIdx = 0;
            for (const auto &minorVersion : majorVersion.minorVersion)
            {
                auto minorVersionElement = new PlatformItem(QString::fromStdString(minorVersion.displayName));
                majorVersionElement->setChild(minorVersionIdx++, 0, minorVersionElement);
            }

            productElement->setChild(majorVersionIdx++, 0, majorVersionElement);
        }

        insertRow(rowCount(), productElement);
    }
}

} // namespace gpui
