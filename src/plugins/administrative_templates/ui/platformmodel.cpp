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
#include "bundle/policyroles.h"

#include <qnamespace.h>

#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>

#include <QDebug>
#include <QSet>
#include <QString>

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
template<>
struct std::hash<QString>
{
    std::size_t operator()(QString const &s) const noexcept { return qHash(s); }
};
#endif

namespace gpui
{
class PlatformModelPrivate
{
public:
    QStandardItemModel *sourceModel = nullptr;
    std::vector<std::shared_ptr<model::admx::SupportedProduct>> items{};
    std::unordered_map<QString, QModelIndex> mapPlatformToNode{};

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
    PlatformItem(std::string sort_key, std::string display);
};

PlatformItem::PlatformItem(std::string sort_key, std::string display)
{
    setData(QString::fromStdString(sort_key));
    setText(QString::fromStdString(display));

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
    const QStandardItem *item = itemFromIndex(index);
    const bool rv             = QStandardItemModel::setData(index, value, role);

    // (un)check all children on item (un)check
    for (int i = 0; i < item->rowCount(); i++)
    {
        QStandardItem *childItem = item->child(i);
        setData(childItem->index(), value, role);
    }

    // update all parents state
    for (QStandardItem *parent = item->parent(); parent; parent = parent->parent())
    {
        QStandardItemModel::setData(parent->index(), getItemCheckStateBasedOnChildren(parent), role);
    }

    return rv;
}

Qt::CheckState PlatformModel::getItemCheckStateBasedOnChildren(const QStandardItem *parent) const
{
    constexpr int NUMBER_OF_VARIANTS       = 3; // Qt::Unchecked, Qt::PartiallyChecked, Qt::Checked
    int32_t hasVariant[NUMBER_OF_VARIANTS] = {0};
    int32_t numberOfChildren               = parent->rowCount();

    for (int32_t row = 0; row < numberOfChildren; row++)
    {
        ++hasVariant[parent->child(row)->checkState()];
    }

    for (int32_t variant = 0; variant < NUMBER_OF_VARIANTS; ++variant)
    {
        if (hasVariant[variant] == numberOfChildren)
        {
            return static_cast<Qt::CheckState>(variant);
        };
    }

    return Qt::PartiallyChecked;
}

void PlatformModel::populateModel(std::vector<std::shared_ptr<model::admx::SupportedProduct>> products)
{
    clear();

    d->items = products;

    for (const auto &product : d->items)
    {
        auto productElement = new PlatformItem(product->name, product->displayName);

        int majorVersionIdx = 0;
        for (const auto &majorVersion : product->majorVersion)
        {
            auto majorVersionElement = new PlatformItem(majorVersion.name, majorVersion.displayName);

            int minorVersionIdx = 0;
            for (const auto &minorVersion : majorVersion.minorVersion)
            {
                auto minorVersionElement = new PlatformItem(minorVersion.name, minorVersion.displayName);

                majorVersionElement->setChild(minorVersionIdx++, 0, minorVersionElement);
            }

            productElement->setChild(majorVersionIdx++, 0, majorVersionElement);
        }

        insertRow(rowCount(), productElement);
    }

    buildPlatformMap();
}

void PlatformModel::buildPlatformMap()
{
    std::function<void(const QModelIndex &root)> buildCache = [&](const QModelIndex &root) {
        const QString &rootReference        = root.data(PLATFORM_ROLE_SORT).value<QString>();
        d->mapPlatformToNode[rootReference] = root;
        for (int row = 0; row < rowCount(root); ++row)
        {
            buildCache(index(row, 0, root));
        }
    };
    buildCache(invisibleRootItem()->index());
}

int PlatformModel::getPlatformIndex(QString platform, QString parentReference) const
{
    const QModelIndex &parentIndex = d->mapPlatformToNode[parentReference];
    QModelIndex platformIndex      = d->mapPlatformToNode[platform];
    if (!parentIndex.isValid() || !platformIndex.isValid())
    {
        return -1;
    }

    while (platformIndex.parent() != parentIndex)
    {
        if (platformIndex == invisibleRootItem()->index())
        {
            return -1;
        }

        platformIndex = platformIndex.parent();
    }

    return platformIndex.row();
}

} // namespace gpui
