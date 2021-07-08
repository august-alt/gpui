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

#ifndef GPUI_POLICYRESOURCES_H
#define GPUI_POLICYRESOURCES_H

#include "../model.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace model
{
    namespace presentation
    {
        class Presentation;

        class PresentationWidget;
        /*!
         * \brief The PolicyResources class Represents container for localized policy resources and display elements.
         *
         * The base type for localized .adml files with a single language/culture per file.
         * Localization files are always in the namespace of the associated .admx file and never reference
         * definitions from other namespaces.
         */
        class GPUI_MODEL_EXPORT PolicyResources
        {
        public:
            /*!
             * \brief displayName The localized friendly name of the policy settings file.
             * Unsupported by current Group Policy tools.
             */
            std::string displayName;

            /*!
             * \brief description The localized description of policy settings contained in an .adml file.
             * Unsupported by current Group Policy tools.
             */
            std::string description;

            /*!
             * \brief annotation A comment string for .adml files.
             * Strings added to this tag will not be processed by Group Policy tools.
             */
            std::vector<std::string> annotation;

            /*!
             * \brief stringTable A table of localized strings.
             */
            std::map<std::string, std::string> stringTable;

            /*!
             * \brief presentationTable A table of presentation elements representing policy setting parameters.
             */
            std::map<std::string, std::shared_ptr<Presentation>> presentationTable;
        };
    }
}

#endif // GPUI_POLICYRESOURCES_H
