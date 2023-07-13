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

#ifndef GPUI_COMMENTS_MODEL_H
#define GPUI_COMMENTS_MODEL_H

#include <QStandardItemModel>

#include <memory>

namespace comments
{

class CommentsModel : public QStandardItemModel
{
public:
    enum CommentsModelRoles
    {
        COMMENT_TEXT_ROLE   = Qt::DisplayRole,
        ITEM_REFERENCE_ROLE = Qt::UserRole + 1,
        ITEM_NAMESPACE_ROLE = Qt::UserRole + 2,
    };

public:
    CommentsModel(QObject* parent = nullptr);

    void load(const QString& path);
    void save(const QString &path, const QString &localeName);

    QModelIndex indexFromItemReference(const QString &itemRef);

    bool setComment(const QString& comment, const QString& policyName, const QString& namespace_);

private:
    CommentsModel(const CommentsModel&)            = delete;   // copy ctor
    CommentsModel(CommentsModel&&)                 = delete;   // move ctor
    CommentsModel& operator=(const CommentsModel&) = delete;   // copy assignment
    CommentsModel& operator=(CommentsModel&&)      = delete;   // move assignment
};

}

#endif  //GPUI_COMMENTS_MODEL_H
