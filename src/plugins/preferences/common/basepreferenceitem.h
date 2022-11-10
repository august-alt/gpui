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

#ifndef GPUI_BASE_PREFERENCE_ITEM_H
#define GPUI_BASE_PREFERENCE_ITEM_H

#include <mvvm/model/compounditem.h>

namespace preferences
{

template <typename T>
class BasePreferenceItem : public ModelView::CompoundItem
{
public:
    BasePreferenceItem(const std::string& modelType)
        : ModelView::CompoundItem(modelType) {}

    template <typename EnumType>
    constexpr static int propertyToInt(const EnumType& type)
    {
        return static_cast<int>(type);
    }

protected:
    template <typename PropertyType, typename EnumType>
    inline void copyProperty(const EnumType& type, const T &other)
    {
        std::string property = other.propertyToString(type);
        addProperty(property, other.template property<PropertyType>(property));
    }
};

}

#endif//GPUI_BASE_PREFERENCE_ITEM_H
