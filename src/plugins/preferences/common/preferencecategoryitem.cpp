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

#include "preferencecategoryitem.h"

#include <QUuid>

namespace preferences
{
PreferenceCategoryItem::PreferenceCategoryItem()
    : ModelView::CompoundItem("PreferenceCategoryItem")
{
    setDisplayName("");

    addProperty(TYPE, std::map<std::string, QString>());
    addProperty(NODE_ID, QUuid::createUuid());
    addProperty(PARENT_ID, QUuid::createUuid());
    addProperty(HELP_MSG, "");
    addProperty(MODEL_TYPE, false);
}

PreferenceCategoryItem::PreferenceCategoryItem(const PreferenceCategoryItem &other)
    : ModelView::CompoundItem("PreferenceCategoryItem")
{
    setDisplayName(other.displayName());

    setProperty(TYPE, other.property<std::map<std::string, QString>>(TYPE));
    setProperty(NODE_ID, other.property<QUuid>(NODE_ID));
    setProperty(PARENT_ID, other.property<QUuid>(PARENT_ID));
    setProperty(HELP_MSG, other.property<std::string>(HELP_MSG));
    setProperty(MODEL_TYPE, other.property<std::string>(MODEL_TYPE));
}

} // namespace preferences
