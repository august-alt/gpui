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

#ifndef SCRIPTITEM_H
#define SCRIPTITEM_H

#include <mvvm/model/compounditem.h>

namespace scripts_plugin
{
class ScriptItem : public ModelView::CompoundItem
{
public:
    enum PropertyType
    {
        PATH      = 0,
        PARAMETER = 1
    };

public:
    //static inline const std::string PATH      = "path";
    //static inline const std::string PARAMETER = "parameter";

    ScriptItem();
    ScriptItem(const ScriptItem &other);

    constexpr static int propertyToInt(PropertyType &type);

    constexpr static const char *propertyToString(const PropertyType &type)
    {
        switch (type)
        {
        case PATH:
            return "path";
        case PARAMETER:
            return "parameter";
        default:
            break;
        }

        return "";
    }
};

} // namespace scripts_plugin

Q_DECLARE_METATYPE(scripts_plugin::ScriptItem)

#endif // SCRIPTITEM_H
