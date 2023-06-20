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

#include "bundle/policyroles.h"

namespace gpui
{
class PlatformModelPrivate
{
public:
    QStandardItemModel *sourceModel = nullptr;
    QSet<QString> uniqueItems{};

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
    setSourceData(sourceModel);
}

PlatformModel::~PlatformModel()
{
    delete d;
}

void PlatformModel::setSourceData(QStandardItemModel *sourceModel)
{
    if (sourceModel && (sourceModel != d->sourceModel))
    {
        d->sourceModel = sourceModel;

        populateModel(d->sourceModel);
    }
}

void PlatformModel::populateModel(QStandardItemModel *sourceModel)
{
    d->uniqueItems.clear();

    clear();

    std::unique_ptr<QStack<QModelIndex>> stack = std::make_unique<QStack<QModelIndex>>();
    stack->push(sourceModel->invisibleRootItem()->index());

    while (!stack->empty())
    {
        auto current = stack->top();
        stack->pop();

        auto supportedOn = current.data(model::bundle::PolicyRoles::SUPPORTED_ON).value<QString>().trimmed();

        if (!supportedOn.isEmpty())
        {
            d->uniqueItems.insert(supportedOn);
        }

        for (int row = 0; row < sourceModel->rowCount(current); ++row)
        {
            QModelIndex index = sourceModel->index(row, 0, current);

            if (sourceModel->hasChildren(index))
            {
                for (int childRow = 0; childRow < sourceModel->rowCount(index); ++childRow)
                {
                    QModelIndex childIndex = sourceModel->index(childRow, 0, index);
                    stack->push(childIndex);
                }
            }
        }
    }

    for (const auto &uniqueItem : d->uniqueItems)
    {
        auto listElement = new QStandardItem(uniqueItem);
        listElement->setCheckable(true);

        insertRow(rowCount(), listElement);
    }
}

} // namespace gpui
