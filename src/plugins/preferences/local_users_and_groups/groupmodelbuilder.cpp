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

#include "groupmodelbuilder.h"

#include "groupmemberscontaineritem.h"
#include "localgroupcontaineritem.h"
#include "localgroupitem.h"
#include "localuseritem.h"

#include "common/commonitem.h"

namespace preferences
{
GroupModelBuilder::GroupModelBuilder()
    : BaseModelBuilder()
{}

std::unique_ptr<PreferencesModel> GroupModelBuilder::schemaToModel(std::unique_ptr<Groups> &group)
{
    // Q_UNUSED(group);
    auto model = std::make_unique<PreferencesModel>();
    // TODO: Implement.
    for (const auto &groupsSchema : group->Group())
    {
        auto sessionItem = model->insertItem<LocalGroupContainerItem<LocalGroupItem>>(model->rootItem());

        for (const auto &properties : groupsSchema.Properties())
        {
            auto localGroup = sessionItem->getLocalGroup();
            auto action     = properties.action();
            auto state      = getActionCheckboxState(action.present() ? action->c_str() : "");
            localGroup.setProperty(LocalGroupItem::ACTION, state);
            localGroup.setProperty<std::string>(LocalGroupItem::GROUP_NAME, properties.groupName());
            localGroup.setProperty<std::string>(LocalGroupItem::GROUP_SID, properties.groupSid()->c_str());
            localGroup.setProperty<std::string>(LocalGroupItem::USER_ACTION,
                                                getOptionalPropertyData(properties.userAction()));
            localGroup.setProperty<std::string>(LocalGroupItem::DESCRIPTION,
                                                getOptionalPropertyData(properties.description()));
            localGroup.setProperty<std::string>(LocalGroupItem::NEW_NAME, getOptionalPropertyData(properties.newName()));
            localGroup.setProperty<std::string>(LocalGroupItem::USER_ACTION,
                                                getOptionalPropertyData(properties.userAction()));
            localGroup.setProperty<bool>(LocalGroupItem::REMOVE_ACCOUNTS,
                                         getOptionalPropertyData(properties.removeAccounts()));
            localGroup.setProperty<bool>(LocalGroupItem::DELETE_ALL_USERS,
                                         getOptionalPropertyData(properties.deleteAllUsers()));
            localGroup.setProperty<bool>(LocalGroupItem::DELETE_ALL_GROUPS,
                                         getOptionalPropertyData(properties.deleteAllGroups()));

            auto common = sessionItem->getCommon();
            setCommonItemData(common, groupsSchema);
        }
    }

    for (const auto &userSchema : group->User())
    {
        auto sessionItem = model->insertItem<LocalGroupContainerItem<LocalUserItem>>(model->rootItem());

        for (const auto &properties : userSchema.Properties())
        {
            auto localUser = sessionItem->getLocalGroup();
            auto action    = properties.action();
            auto state     = getActionCheckboxState(action.present() ? action->c_str() : "");
            localUser.setProperty(LocalUserItem::ACTION, state);
            localUser.setProperty<std::string>(LocalUserItem::USER_NAME, properties.userName());
            localUser.setProperty<std::string>(LocalUserItem::NEW_NAME, properties.newName());
            localUser.setProperty<std::string>(LocalUserItem::FULL_NAME, properties.fullName());
            localUser.setProperty<std::string>(LocalUserItem::DESCRIPTION,
                                               getOptionalPropertyData(properties.description()));
            localUser.setProperty<std::string>(LocalUserItem::CPASSWORD,
                                               getOptionalPropertyData(properties.cpassword()));
            localUser.setProperty<bool>(LocalUserItem::CHANGE_LOGON, getOptionalPropertyData(properties.changeLogon()));
            localUser.setProperty<bool>(LocalUserItem::ACCOUNT_DISABLED,
                                        getOptionalPropertyData(properties.acctDisabled()));
            localUser.setProperty<bool>(LocalUserItem::PASSWORD_NEVER_EXPIRES,
                                        getOptionalPropertyData(properties.changeLogon()));
            localUser.setProperty<QDate>(LocalUserItem::EXPIRES,
                                         QDate::fromString(getOptionalPropertyData(properties.expires()).c_str()));
            localUser.setProperty<bool>(LocalUserItem::NO_CHANGE, getOptionalPropertyData(properties.noChange()));
            localUser.setProperty<bool>(LocalUserItem::ACCOUNT_NEVER_EXPIRES,
                                        getOptionalPropertyData(properties.neverExpires()));

            auto common = sessionItem->getCommon();
            setCommonItemData(common, userSchema);
        }
    }

    return model;
}

std::unique_ptr<Groups> GroupModelBuilder::modelToSchema(std::unique_ptr<PreferencesModel> &model)
{
    auto groups = std::make_unique<Groups>("{3125E937-EB16-4b4c-9934-544FC6D24D26}");

    for (const auto &containerItem : model->topItems())
    {
        if (auto groupContainer = dynamic_cast<LocalGroupContainerItem<LocalGroupItem> *>(containerItem); groupContainer)
        {
            auto groupModel  = groupContainer->getLocalGroup();
            auto commonModel = groupContainer->getCommon();

            auto group = createRootElement<Group_t>("{6D4A79E4-529C-4481-ABD0-F5BD7EA93BA7}");

            auto properties = GroupProperties_t(groupModel.property<std::string>(LocalGroupItem::GROUP_NAME));
            properties.action(getActionCheckboxModel(groupModel.property<int>(LocalGroupItem::ACTION)));
            properties.groupSid(groupModel.property<std::string>(LocalGroupItem::GROUP_SID));
            properties.newName(groupModel.property<std::string>(LocalGroupItem::NEW_NAME));
            properties.description(groupModel.property<std::string>(LocalGroupItem::DESCRIPTION));
            properties.userAction(groupModel.property<std::string>(LocalGroupItem::USER_ACTION));
            properties.removeAccounts(groupModel.property<bool>(LocalGroupItem::REMOVE_ACCOUNTS));
            properties.deleteAllUsers(groupModel.property<bool>(LocalGroupItem::DELETE_ALL_USERS));
            properties.deleteAllGroups(groupModel.property<bool>(LocalGroupItem::DELETE_ALL_GROUPS));

            setCommonModelData(group, commonModel);
            group.Properties().push_back(properties);

            //TODO: Implement members
        }

        if (auto userContainer = dynamic_cast<LocalGroupContainerItem<LocalUserItem> *>(containerItem); userContainer)
        {
            auto userModel   = userContainer->getLocalGroup();
            auto commonModel = userContainer->getCommon();

            auto user       = createRootElement<User_t>("{DF5F1855-51E5-4d24-8B1A-D9BDE98BA1D1}");
            auto properties = UserProperties_t(userModel.property<std::string>(LocalUserItem::NEW_NAME),
                                               userModel.property<std::string>(LocalUserItem::FULL_NAME),
                                               userModel.property<std::string>(LocalUserItem::USER_NAME));
            properties.description(userModel.property<std::string>(LocalUserItem::DESCRIPTION));
            properties.cpassword(userModel.property<std::string>(LocalUserItem::CPASSWORD));
            properties.changeLogon(userModel.property<bool>(LocalUserItem::CHANGE_LOGON));
            properties.acctDisabled(userModel.property<bool>(LocalUserItem::ACCOUNT_DISABLED));
            properties.neverExpires(userModel.property<bool>(LocalUserItem::ACCOUNT_NEVER_EXPIRES));
            properties.expires(userModel.property<QDate>(LocalUserItem::EXPIRES).toString().toStdString());
            properties.noChange(userModel.property<bool>(LocalUserItem::NO_CHANGE));

            setCommonModelData(user, commonModel);
            user.Properties().push_back(properties);
        }
    }

    return groups;
}

} // namespace preferences
