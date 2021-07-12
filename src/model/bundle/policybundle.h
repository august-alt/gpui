/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd.
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

#ifndef GPUI_POLICYBUNDLE_H
#define GPUI_POLICYBUNDLE_H

#include "../model.h"
#include "policytreemodel.h"

#include <memory>
#include <string>

class QFileInfo;
class QString;
class QStandardItem;
class QStandardItemModel;

namespace model
{
    namespace admx {
        class Policy;
    }

    namespace bundle
    {
        class PolicyBundlePrivate;

        class GPUI_MODEL_EXPORT PolicyBundle
        {
        public:
            PolicyBundle();
            ~PolicyBundle();

            std::unique_ptr<QStandardItemModel> loadFolder(const std::string& path, const std::string& language,
                                                        const std::string& fallbackLanguage);

        private:
            bool loadAdmxAndAdml(const QFileInfo &admxFileName, const std::string& language,
                                 const std::string& fallbackLanguage);

            QString constructFileName(const QFileInfo &fileName, const std::string &language);

            PolicyBundlePrivate* d;

            void rearrangeTreeItems();
            void assignParentCategory(const std::string& rawCategory, QStandardItem *machineItem, QStandardItem* userItem);

            QStandardItem *createItem(const QString& displayName, const QString &iconName, const QString &explainText,
                                      const uint itemType = 0);
        };
    }
}

#endif // GPUI_POLICYBUNDLE_H
