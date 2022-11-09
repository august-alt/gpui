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

#ifndef GPUI_SHARES_CONTAINER_ITEM_H
#define GPUI_SHARES_CONTAINER_ITEM_H

#include <mvvm/model/compounditem.h>

#include "interfaces/containeriteminterface.h"

namespace preferences
{
//! Folder item representation for editor.

class CommonItem;
class SharesItem;

class SharesContainerItem : public ModelView::CompoundItem, public ContainerItemInterface
{
public:
    static inline const std::string NAME                     = "name";
    static inline const std::string ORDER                    = "order";
    static inline const std::string ACTION                   = "action";
    static inline const std::string PATH                     = "path";
    static inline const std::string USER_LIMIT               = "userLimit";
    static inline const std::string ACCESS_BASED_ENUMERATION = "abe";

    static inline const std::string COMMON = "common";
    static inline const std::string SHARES = "shares";

    explicit SharesContainerItem();

    CommonItem *getCommon() const;
    void setCommon(const CommonItem &item);

    SharesItem *getShares() const;
    void setShares(const SharesItem &item);

    void setupListeners() override;

    void retranslateStrings() override;
};

} // namespace preferences

#endif //GPUI_SHARES_CONTAINER_ITEM_H
