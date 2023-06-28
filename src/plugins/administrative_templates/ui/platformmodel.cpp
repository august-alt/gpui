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

#include <memory>

#include <QSet>
#include <QStack>
#include <QString>

#include <QDebug>

#include "bundle/policyroles.h"

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

PlatformModel::PlatformModel(QStandardItemModel *sourceModel)
    : QStandardItemModel()
    , d(new PlatformModelPrivate())
{
    setSourceData(sourceModel, {});
}

PlatformModel::~PlatformModel()
{
    delete d;
}

void PlatformModel::setSourceData(QStandardItemModel *sourceModel,
                                  std::vector<std::shared_ptr<model::admx::SupportedProduct>> products)
{
    if (sourceModel && (sourceModel != d->sourceModel))
    {
        d->sourceModel = sourceModel;

        populateModel(d->sourceModel, products);
    }
}

void PlatformModel::populateModel(QStandardItemModel *sourceModel,
                                  std::vector<std::shared_ptr<model::admx::SupportedProduct>> products)
{
    (void) sourceModel;
    clear();

    d->items = products;

    for (const auto &product : d->items)
    {
        auto productElement = new QStandardItem(QString::fromStdString(product->displayName));
        productElement->setEditable(false);
        productElement->setCheckable(true);

        int majorVersionIdx = 0;
        for (const auto &majorVersion : product->majorVersion)
        {
            auto majorVersionElement = new QStandardItem(QString::fromStdString(majorVersion.displayName));
            majorVersionElement->setEditable(false);
            majorVersionElement->setCheckable(true);

            int minorVersionIdx = 0;
            for (const auto &minorVersion : majorVersion.minorVersion)
            {
                auto minorVersionElement = new QStandardItem(QString::fromStdString(minorVersion.displayName));
                minorVersionElement->setEditable(false);
                minorVersionElement->setCheckable(true);

                majorVersionElement->setChild(minorVersionIdx++, 0, minorVersionElement);
            }

            productElement->setChild(majorVersionIdx++, 0, majorVersionElement);
        }

        insertRow(rowCount(), productElement);
    }
}

} // namespace gpui
