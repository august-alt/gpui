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

#ifndef GPUI_BASE_MODEL_BUILDER_H
#define GPUI_BASE_MODEL_BUILDER_H

#include <string>

#include "commonitem.h"

#include <QDate>
#include <QString>

namespace preferences
{
class BaseModelBuilder
{
public:
    virtual ~BaseModelBuilder() = default;

protected:
    BaseModelBuilder() = default;

    BaseModelBuilder(const BaseModelBuilder &) = delete;            // copy ctor
    BaseModelBuilder(BaseModelBuilder &&)      = delete;            // move ctor
    BaseModelBuilder &operator=(const BaseModelBuilder &) = delete; // copy assignment
    BaseModelBuilder &operator=(BaseModelBuilder &&) = delete;      // move assignment

    template<template<typename> typename OptionalType, typename T>
    T getOptionalPropertyData(const OptionalType<T> &data)
    {
        if (data.present())
        {
            return data.get();
        }
        else
        {
            return T();
        }
    }

    int getActionCheckboxState(const std::string &data);
    std::string getActionCheckboxModel(const int &data);

    template<typename SchemaType>
    void setCommonItemData(CommonItem *common, const SchemaType &schema)
    {
        common->setProperty(CommonItem::propertyToString(CommonItem::CLSID), schema.clsid().c_str());
        common->setProperty(CommonItem::propertyToString(CommonItem::NAME), schema.name().c_str());
        common->setProperty(CommonItem::propertyToString(CommonItem::STATUS),
                            getOptionalPropertyData(schema.status()).c_str());
        common->setProperty(CommonItem::propertyToString(CommonItem::IMAGE),
                            static_cast<int>(getOptionalPropertyData(schema.image())));
        common->setProperty(CommonItem::propertyToString(CommonItem::CHANGED),
                            getOptionalPropertyData(schema.changed()).c_str());
        common->setProperty(CommonItem::propertyToString(CommonItem::UID), schema.uid().c_str());
        common->setProperty(CommonItem::propertyToString(CommonItem::DESC),
                            getOptionalPropertyData(schema.desc()).c_str());
        common->setProperty(CommonItem::propertyToString(CommonItem::BYPASS_ERRORS),
                            static_cast<bool>(getOptionalPropertyData(schema.bypassErrors())));
        common->setProperty(CommonItem::propertyToString(CommonItem::USER_CONTEXT),
                            static_cast<bool>(getOptionalPropertyData(schema.userContext())));
        common->setProperty(CommonItem::propertyToString(CommonItem::REMOVE_POLICY),
                            static_cast<bool>(getOptionalPropertyData(schema.removePolicy())));
    }

    template<typename CommonData>
    void setCommonModelData(CommonData &data, const preferences::CommonItem *commonModel)
    {
        data.clsid(commonModel->property<std::string>(CommonItem::propertyToString(CommonItem::CLSID)));
        data.name(commonModel->property<std::string>(CommonItem::propertyToString(CommonItem::NAME)));
        data.status(commonModel->property<std::string>(CommonItem::propertyToString(CommonItem::STATUS)));
        data.image(static_cast<uchar>(commonModel->property<int>(CommonItem::propertyToString(CommonItem::IMAGE))));
        data.changed(commonModel->property<std::string>(CommonItem::propertyToString(CommonItem::CHANGED)));
        data.uid(commonModel->property<std::string>(CommonItem::propertyToString(CommonItem::UID)));
        data.desc(commonModel->property<std::string>(CommonItem::propertyToString(CommonItem::DESC)));
        data.bypassErrors(commonModel->property<bool>(CommonItem::propertyToString(CommonItem::BYPASS_ERRORS)));
        data.userContext(commonModel->property<bool>(CommonItem::propertyToString(CommonItem::USER_CONTEXT)));
        data.removePolicy(commonModel->property<bool>(CommonItem::propertyToString(CommonItem::REMOVE_POLICY)));
    }

    template<typename T>
    T createRootElement(const std::string &guid)
    {
        QString dateOfChange(QDate::currentDate().toString(Qt::ISODate) + " "
                             + QTime::currentTime().toString("hh:mm:ss"));

        return T(guid, dateOfChange.toStdString(), "");
    }

    std::string createDateOfChange()
    {
        QString dateOfChange(QDate::currentDate().toString(Qt::ISODate) + " "
                             + QTime::currentTime().toString("hh:mm:ss"));
        return dateOfChange.toStdString();
    }
};

} // namespace preferences

#endif //GPUI_BASE_MODEL_BUILDER_H
