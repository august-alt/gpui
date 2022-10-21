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

#include "sharesmodelbuilder.h"

#include "sharescontaineritem.h"
#include "sharesitem.h"

#include "common/commonitem.h"

namespace preferences
{
SharesModelBuilder::SharesModelBuilder()
    : BaseModelBuilder()
{}

std::unique_ptr<PreferencesModel> SharesModelBuilder::schemaToModel(std::unique_ptr<NetworkShareSettings> &shares)
{
    auto model = std::make_unique<PreferencesModel>();

    for (const auto &sharesSchema : shares->NetShare())
    {
        for (const auto &properties : sharesSchema.Properties())
        {
            auto sessionItem = model->insertItem<SharesContainerItem>(model->rootItem());
            sessionItem->setupListeners();

            auto shareItem = sessionItem->getShares();
            shareItem->setProperty(SharesItem::ACTION,
                                   getActionCheckboxState(getOptionalPropertyData(properties.action()).c_str()));
            shareItem->setProperty(SharesItem::NAME, properties.name().c_str());
            shareItem->setProperty(SharesItem::PATH, getOptionalPropertyData(properties.path()).c_str());
            shareItem->setProperty(SharesItem::COMMENT, getOptionalPropertyData(properties.comment()).c_str());
            shareItem->setProperty(SharesItem::ALL_REGULAR, getOptionalPropertyData(properties.allRegular()));
            shareItem->setProperty(SharesItem::ALL_HIDDEN, getOptionalPropertyData(properties.allHidden()));
            shareItem->setProperty(SharesItem::ALL_ADMIN_DRIVE, getOptionalPropertyData(properties.allAdminDrive()));
            shareItem->setProperty(SharesItem::LIMIT_USERS, getOptionalPropertyData(properties.limitUsers()).c_str());
            shareItem->setProperty(SharesItem::USER_LIMIT,
                                   static_cast<int>(getOptionalPropertyData(properties.userLimit())));
            shareItem->setProperty(SharesItem::ACCESS_BASED_ENUMERATION,
                                   getOptionalPropertyData(properties.abe()).c_str());

            auto common = sessionItem->getCommon();
            setCommonItemData(common, sharesSchema);
        }
    }

    return model;
}

std::unique_ptr<NetworkShareSettings> SharesModelBuilder::modelToSchema(std::unique_ptr<PreferencesModel> &model)
{
    auto shares = std::make_unique<NetworkShareSettings>("{520870D8-A6E7-47e8-A8D8-E6A4E76EAEC2}");

    for (const auto &containerItem : model->topItems())
    {
        if (auto variablesContainer = dynamic_cast<SharesContainerItem *>(containerItem); variablesContainer)
        {
            auto sharesModel = variablesContainer->getShares();
            auto commonModel = variablesContainer->getCommon();

            auto share = NetShare_t("", "", "");
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::CLSID),
                                     "{2888C5E7-94FC-4739-90AA-2C1536D68BC0}");
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::CHANGED), createDateOfChange());

            auto properties = ShareProperties_t(sharesModel->property<std::string>(SharesItem::NAME));
            properties.comment(sharesModel->property<std::string>(SharesItem::COMMENT));
            properties.path(sharesModel->property<std::string>(SharesItem::PATH));
            properties.action(getActionCheckboxModel(sharesModel->property<std::string>(SharesItem::ACTION)));
            properties.allRegular(sharesModel->property<bool>(SharesItem::ALL_REGULAR));
            properties.allHidden(sharesModel->property<bool>(SharesItem::ALL_HIDDEN));
            properties.allAdminDrive(sharesModel->property<bool>(SharesItem::ALL_ADMIN_DRIVE));
            properties.limitUsers(sharesModel->property<std::string>(SharesItem::LIMIT_USERS));
            properties.userLimit(sharesModel->property<int>(SharesItem::USER_LIMIT));
            properties.abe(sharesModel->property<std::string>(SharesItem::ACCESS_BASED_ENUMERATION));

            setCommonModelData(share, commonModel);
            share.Properties().push_back(properties);

            shares->NetShare().push_back(share);
        }
    }

    return shares;
}

} // namespace preferences
