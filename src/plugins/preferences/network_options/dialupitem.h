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

#ifndef GPUI_DIAL_UP_ITEM_H
#define GPUI_DIAL_UP_ITEM_H

#include <mvvm/model/compounditem.h>

namespace preferences
{

//! Drives item representation for editor.

class DialUpItem : public ModelView::CompoundItem
{
public:
    enum PropertyType
    {
        ACTION       = 0,
        USER         = 1,
        NAME         = 2,
        PHONE_NUMBER = 3,
        SYSTEM       = 4
    };

public:
    DialUpItem();
    DialUpItem(const DialUpItem &other);

    constexpr static int propertyToInt(const PropertyType& type);
    constexpr static const char* propertyToString(const PropertyType& type)
    {
        switch (type)
        {
        case ACTION:
            return "action";
        case PHONE_NUMBER:
            return "phoneNumber";
        case NAME:
            return "name";
        case USER:
            return "user";
        case SYSTEM:
            return "system";
        }

        return "";
    }

private:
    template <typename T>
    inline void copyProperty(const PropertyType& type, const DialUpItem &other);
};

template<typename T>
void DialUpItem::copyProperty(const DialUpItem::PropertyType &type, const DialUpItem &other)
{
    addProperty(propertyToString(type), other.property<T>(propertyToString(type)));
}

inline constexpr int DialUpItem::propertyToInt(const DialUpItem::PropertyType &type)
{
    return static_cast<int>(type);
}

}

Q_DECLARE_METATYPE(::preferences::DialUpItem)

#endif//GPUI_DIAL_UP_ITEM_H
