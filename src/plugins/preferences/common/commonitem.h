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

#ifndef GPUI_COMMONITEM_H
#define GPUI_COMMONITEM_H

#include "basepreferenceitem.h"

namespace preferences
{

//! Common item representation for editor.

class CommonItem : public BasePreferenceItem<CommonItem>
{
public:
    enum PropertyType
    {
        CLSID           = 0,
        DISABLED        = 1,
        NAME            = 2,
        STATUS          = 3,
        IMAGE           = 4,
        CHANGED         = 5,
        UID             = 6,
        DESC            = 7,
        BYPASS_ERRORS   = 8,
        USER_CONTEXT    = 9,
        REMOVE_POLICY   = 10,
    };

public:
    CommonItem();
    CommonItem(const CommonItem &other);

    QString name() const;
    void setName(const QString& path);

    std::string changed() const;
    void setChanged(bool state);

    QString desc() const;
    void setDesc(const QString& path);

    bool bypassErrors() const;
    void setBypassErrors(bool state);

    bool userContext() const;
    void setUserContext(bool state);

    bool removePolicy() const;
    void setRemovePolicy(bool state);

    constexpr static inline const char* propertyToString(const PropertyType& type)
    {
        switch (type)
        {
        case CLSID        : return "clsid";
        case DISABLED     : return "disabled";
        case NAME         : return "name";
        case STATUS       : return "status";
        case IMAGE        : return "image";
        case CHANGED      : return "changed";
        case UID          : return "uid";
        case DESC         : return "desc";
        case BYPASS_ERRORS: return "bypassErrors";
        case USER_CONTEXT : return "userContext ";
        case REMOVE_POLICY: return "removePolicy";
        }
        return "";
    }
};

}

Q_DECLARE_METATYPE(::preferences::CommonItem)

#endif//GPUI_COMMONITEM_H
