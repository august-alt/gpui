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

#ifndef GPUI_POLICYDEFINITIONS_H
#define GPUI_POLICYDEFINITIONS_H

#include "../model.h"

#include "category.h"
#include "policy.h"
#include "resources.h"
#include "supportedon.h"

#include <memory>
#include <vector>

namespace model {
    namespace admx {
        /*!
         * \brief The PolicyDefinitions class The PolicyDefinitions element is the document element for an .admx file
         * that defines a set of registry-policy settings. The PolicyDefinitions element also declares a default
         * namespace for all of the elements in the .admx file.
         */
        class GPUI_MODEL_EXPORT PolicyDefinitions
        {
        public:
            /*!
             * \brief revision Version of the .admx file for tracking purposes.
             */
            std::string revision;

            /*!
             * \brief schemaVersion Schema version used by the Group Policy tools.
             * For Windows Vista and Windows Server 2008 schemaVersion must be set as follows; schemaVersion="1.0"
             */
            std::string schemaVersion;

            /*!
             * \brief policyNamespaces Contains mappings of .admx files to unique namespaces.
             */
            int policyNamespaces;

            /*!
             * \brief supersededAdm Reference to the file name for an .adm file.
             * Editor will not read any .adm file designated as superseded.
             */
            std::vector<std::string> supersededAdm;

            /*!
             * \brief resources Contains the required minimum revision of the .adml file.
             */
            Resources resources;

            /*!
             * \brief supportedOn Mapping of products and definitions.
             */
            std::shared_ptr<SupportedOn> supportedOn;

            /*!
             * \brief categories Contains a list of categories under which Group Policy
             * settings will be displayed in the Editor.
             */
            std::vector<std::shared_ptr<Category>> categories;

            /*!
             * \brief policies Contains a list of policy setting definitions.
             */
            std::vector<std::shared_ptr<Policy>> policies;
        };
    }
}

#endif // GPUI_POLICYDEFINITIONS_H
