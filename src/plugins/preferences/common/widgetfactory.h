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

#ifndef GPUI_WIDGET_FACTORY_H
#define GPUI_WIDGET_FACTORY_H

#include <QtWidgets>

#include "interfaces/preferenceswidgetinterface.h"
#include "interfaces/widgetfactoryinterface.h"

#include <memory>

namespace preferences
{
    class WidgetFactoryPrivate;

    class WidgetFactory final : public WidgetFactoryInterface
    {
    public:
        WidgetFactory();
        ~WidgetFactory();

        void registerItem(const std::string &type, std::function<std::unique_ptr<PreferenceWidgetInterface>()> func) override;

        std::vector<std::unique_ptr<PreferenceWidgetInterface> > create(const std::string& type) const override;

    private:
        WidgetFactory(const WidgetFactory& other) = delete;
        WidgetFactory& operator=(const WidgetFactory& other) = delete;

    private:
        std::unique_ptr<WidgetFactoryPrivate> d;
    };
}

#endif//GPUI_WIDGET_FACTORY_H
