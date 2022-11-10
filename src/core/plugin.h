/***********************************************************************************************************************
**
** Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>
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

#ifndef GPUI_PLUGIN_H
#define GPUI_PLUGIN_H

#include "core.h"

#include "common.h"

#include <functional>
#include <map>
#include <memory>

class QLibrary;
class QString;

namespace gpui
{
class PluginPrivate;
class PluginStorage;

class GPUI_CORE_EXPORT Plugin
{
public:
    virtual ~Plugin();

    const QString &getName() const;

    void setLibrary(std::unique_ptr<QLibrary> library);
    QLibrary *getLibrary() const;

    const std::map<QString, std::function<void *()>> &getPluginClasses() const;

protected:
    explicit Plugin(const QString &name);
    explicit Plugin(const char *name);

    void registerPluginClass(const QString &name, std::function<void *()> constructor);

private:
    Plugin(const Plugin &) = delete;            // copy ctor
    Plugin(Plugin &&)      = delete;            // move ctor
    Plugin &operator=(const Plugin &) = delete; // copy assignment
    Plugin &operator=(Plugin &&) = delete;      // move assignment

private:
    PluginPrivate *d;
};
} // namespace gpui

#define GPUI_EXPORT_PLUGIN(name, className) \
    extern "C" GPUI_SYMBOL_EXPORT ::gpui::Plugin *gpui_plugin_init() { return new className; }

#define GPUI_REGISTER_PLUGIN_CLASS(name, pluginClass) registerPluginClass(name, []() { return new pluginClass(); })

#endif // GPUI_PLUGIN_H
