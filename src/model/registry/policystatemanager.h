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

#ifndef GPUI_POLICY_STATE_MANAGER_H
#define GPUI_POLICY_STATE_MANAGER_H

#include "../model.h"

#include <string>

class QVariant;

namespace model {
    namespace admx {
        class AbstractRegistryValue;
        class Policy;        
    }

    namespace registry {
        class AbstractRegistrySource;
        class PolicyStateManagerPrivate;

        class GPUI_MODEL_EXPORT PolicyStateManager
        {
        public:
            enum PolicyState
            {
                STATE_NOT_CONFIGURED = 0,
                STATE_ENABLED        = 1,
                STATE_DISABLED       = 2
            };

        public:
            PolicyStateManager(AbstractRegistrySource& source, const admx::Policy& policy);
            ~PolicyStateManager();

            PolicyState determinePolicyState() const;

            void setupPolicyState(PolicyState state);

        private:
            bool determineIfPolicyEnabled() const;
            bool determineIfPolicyDisabled() const;

            void setPolicyStateEnabled();
            void setPolicyStateDisabled();
            void setPolicyStateNotConfigured();

            bool checkValueState(const std::string &key, const std::string &valueName,
                                 model::admx::AbstractRegistryValue* registryValue) const;
            void setValueState(const std::string &key, const std::string &valueName,
                               model::admx::AbstractRegistryValue* registryValue);

        private:
            PolicyStateManagerPrivate* d;
        };
    }
}


#endif // GPUI_POLICY_STATE_MANAGER_H
