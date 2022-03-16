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

#ifndef GPUI_PRESENTATIONBUILDER_H
#define GPUI_PRESENTATIONBUILDER_H

#include "gui.h"

#include <QWidget>
#include <QDialogButtonBox>

class QVBoxLayout;

namespace model {
    namespace presentation {
        class Presentation;
    }
    namespace admx {
        class Policy;
    }
    namespace registry {
        class AbstractRegistrySource;
    }
    namespace command {
        class CommandGroup;
    }
}

namespace gui
{
    struct GPUI_GUI_EXPORT PresentationBuilderParams final
    {
        const model::presentation::Presentation& presentation;
        const model::admx::Policy& policy;
        model::registry::AbstractRegistrySource& source;
        QPushButton& saveButton;
        bool& dataChanged;
        bool& stateEnabled;
    };

    class PresentationBuilderPrivate;

    /*!
     * \class PresentationBuilder
     * \brief The PresentationBuilder class
     *
     * \ingroup gui
     */
    class GPUI_GUI_EXPORT PresentationBuilder
    {
    public:
        /*!
         * @brief build
         * @param presentation Defines a reference to policy representation.
         * @return nullptr if build failed, and widget associated with policy otherwise.
         */
        static QVBoxLayout *build(const PresentationBuilderParams& params);

    private:
        static PresentationBuilderPrivate* d;
    };
}

#endif  // GPUI_PRESENTATIONBUILDER_H
