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

#ifndef GPUI_MODEL_BUILDER_BASE_H
#define GPUI_MODEL_BUILDER_BASE_H

#include <memory>

#include <QStandardItemModel>
#include <QStringList>

#include "commonroles.h"

namespace gpui
{
    class ModelBuilderBase
    {
    protected:
        QStandardItem *itemCreator(const std::string &field);

        QStandardItem *itemCreator(const unsigned char &field);

        template <typename T>
        void createItemIfExist(const T& field, int index, int role, std::unique_ptr<QStandardItemModel>& model);

        template <typename T>
        void createItem(T& field, int index, int role, std::unique_ptr<QStandardItemModel>& model);

        template <typename T>
        void createComboBoxValue(T& field, int index, int role, std::unique_ptr<QStandardItemModel>& model,
                                 const QStringList& possibleValue);

        template <typename T>
        void createBooleanValue(T& field, int index, int role, std::unique_ptr<QStandardItemModel>& model,
                                bool invert = false);

        template <typename T>
        void setStandardValues(T& preference, int index, std::unique_ptr<QStandardItemModel>& model);
    };
}

#include "modelbuilderbase.inl"

#endif // GPUI_MODEL_BUILDER_BASE_H
