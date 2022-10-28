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

#include "pluginstorage.h"
#include "plugin.h"

#include <QDir>
#include <QLibrary>

#include <map>
#include <memory>

namespace gpui {

class PluginStoragePrivate
{
public:    
    std::map<QString, std::unique_ptr<Plugin> > pluginMap = {};
    std::map<QString, std::map<QString, std::function<void*()> > > classMap = {};
};

PluginStorage::PluginStorage()
    : d(new PluginStoragePrivate())
{    
}

PluginStorage::~PluginStorage()
{
    for (auto& plugin : d->pluginMap)
    {
        unloadPlugin(plugin.first);
    }

    delete d;
}

Plugin* PluginStorage::getPlugin(const QString& pluginName)
{
    auto search = d->pluginMap.find(pluginName);
    if (search != d->pluginMap.end()) {
        return search->second.get();
    }

    return nullptr;
}

PluginStorage* PluginStorage::instance()
{    
    static PluginStorage instance;

    return &instance;
}

bool PluginStorage::loadPlugin(const QFileInfo& fileName)
{
    std::unique_ptr<QLibrary> pluginLibrary = std::make_unique<QLibrary>(fileName.absoluteFilePath());
    if (!pluginLibrary->load())
    {
        // TODO: Log results.

        return false;
    }

    typedef gpui::Plugin* (*gpui_plugin_init)();
    gpui_plugin_init initFunction = reinterpret_cast<gpui_plugin_init>(pluginLibrary->resolve("gpui_plugin_init"));

    if (!initFunction)
    {
        // TODO: Log results.

        return false;
    }

    Plugin* plugin = initFunction();

    if (!plugin) {
        // TODO: Log results.

        return false;
    }

    plugin->setLibrary(std::move(pluginLibrary));

    for (const auto& entry : plugin->getPluginClasses())
    {
        registerPluginClass(plugin->getName(), entry.first, entry.second);
    }

    delete plugin;

    return true;
}

void PluginStorage::loadPluginDirectory(const QString& directoryName)
{
    QDir directory(directoryName);
    const QFileInfoList files = directory.entryInfoList();

    for (const QFileInfo& file : files) {
        loadPlugin(file);
    }
}

bool PluginStorage::unloadPlugin(const QString& pluginName)
{
    auto search = d->pluginMap.find(pluginName);
    if (search != d->pluginMap.end())
    {
        for (const auto& entry : search->second->getPluginClasses())
        {
            unregisterPluginClass(search->second->getName(), entry.first);
        }

        search->second->getLibrary()->unload();
        d->pluginMap.erase(search);
        return true;
    }

    return false;
}

bool PluginStorage::unloadPlugin(Plugin* plugin)
{
    if (plugin)
    {
        return unloadPlugin(plugin->getName());
    }

    return false;
}

void PluginStorage::loadDefaultPlugins()
{    
    loadPluginDirectory("/usr/lib/gpui/plugins/");
    loadPluginDirectory("/usr/lib64/gpui/plugins/");

    QString customPluginDir = qgetenv("GPUI_PLUGIN_DIRECTORY");
    if (!customPluginDir.isEmpty())
    {
        loadPluginDirectory(customPluginDir);
    }
}

void PluginStorage::registerPluginClass(const QString& pluginName, const QString& className, std::function<void*()> constructor)
{
    auto search = d->classMap.find(pluginName);
    if (search == d->classMap.end())
    {
        d->classMap[pluginName] = std::map<QString, std::function<void*()> >();
    }
    std::map<QString, std::function<void*()> >& pluginConstructors = d->classMap[pluginName];
    pluginConstructors[className] = constructor;
}

bool PluginStorage::unregisterPluginClass(const QString& pluginName, const QString& className)
{
    auto plugin = d->classMap.find(pluginName);
    if (plugin != d->classMap.end())
    {
        auto search = plugin->second.find(className);
        if (search != plugin->second.end()) {
            d->classMap[pluginName].erase(search);
            return true;
        }
    }

    return false;
}

void* PluginStorage::createPluginClass(const QString &className, const QString &pluginName)
{
    auto search = d->classMap.find(pluginName);
    if (search != d->classMap.end())
    {
        return search->second[className]();
    }

    return nullptr;
}

}
