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

#include <QScopedPointer>
#include <QSet>
#include <QStack>
#include <QString>

namespace gpui
{
class PlatformModelPrivate
{
public:
    QStandardItemModel *sourceModel = nullptr;
    QSet<QString> uniqueItems{};
};

PlatformModel::PlatformModel(QStandardItemModel *sourceModel)
    : QStandardItemModel()
    , d(new PlatformModelPrivate())
{
    setSourceData(sourceModel);
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

    QScopedPointer<QStack<QModelIndex>> stack;
    stack->push(sourceModel->invisibleRootItem()->index());

    while (!stack->empty())
    {
        auto current = stack->top();
        stack->pop();

        d->uniqueItems.insert(current.data().value<QString>());

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
        insertRow(rowCount(), new QStandardItem(uniqueItem));
    }
}

} // namespace gpui
