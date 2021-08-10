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

#ifndef GPUI_ABSTRACT_REGISTRY_SOURCE_H
#define GPUI_ABSTRACT_REGISTRY_SOURCE_H

#include "../model.h"

#include "registrysourcetype.h"
#include "registryentrytype.h"

#include <QVariant>

namespace model {
    namespace registry {
        /*!
         * \brief The AbstractRegistrySource class Represents base of registry source.
         */
        class GPUI_MODEL_EXPORT AbstractRegistrySource
        {
        public:
            virtual QVariant getValue(const std::string& key, const std::string& valueName) const = 0;
            virtual void setValue(const std::string& key, const std::string& valueName,
                                  RegistryEntryType type, const QVariant& data) = 0;

            virtual bool isValuePresent(const std::string& key, const std::string& valueName) const = 0;

            virtual void markValueForDeletion(const std::string& key, const std::string& valueName) = 0;
            virtual void undeleteValue(const std::string& key, const std::string& valueName) = 0;

            virtual std::vector<std::string> getValueNames(const std::string& key) const = 0;
            virtual void clearKey(const std::string& key) = 0;

            RegistrySourceType getType() const;

            virtual ~AbstractRegistrySource() = default;

        protected:
            AbstractRegistrySource(RegistrySourceType type);

        private:
            RegistrySourceType type;
        };
    }
}

#endif // GPUI_ABSTRACT_REGISTRY_SOURCE_H
