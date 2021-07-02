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

#ifndef GPUI_PLUGINSTORAGE_H
#define GPUI_PLUGINSTORAGE_H

#include "model.h"

#include <QString>

#include <functional>
#include <vector>

class QFileInfo;

namespace gpui {

class Plugin;
class PluginStoragePrivate;

/*!
 * \class PluginStorage
 * \brief The PluginStorage class
 *
 * \ingroup gpui
 */
class GPUI_MODEL_EXPORT PluginStorage
{
public:
    /*!
     * \brief getPlugin
     * \param pluginName
     * \return
     */
    Plugin *getPlugin(const QString& pluginName);

    /*!
     * \brief loadPlugin
     * \param fileName
     * \return
     */
    bool loadPlugin(const QFileInfo &fileName);

    /*!
     * \brief loadPluginDirectory
     * \param directoryName
     */
    void loadPluginDirectory(const QString& directoryName);

    /*!
     * \brief unloadPlugin
     * \param pluginName
     * \return
     */
    bool unloadPlugin(const QString& pluginName);

    /*!
     * \brief unloadPlugin
     * \param plugin
     * \return
     */
    bool unloadPlugin(Plugin* plugin);

    /**
     * @brief loadDefaultPlugins
     */
    void loadDefaultPlugins();

    /*!
     *  \brief createPluginClass
     */
    template<typename T>
    T* createPluginClass(const QString& pluginName)
    {
        return reinterpret_cast<T*>(createPluginClass(typeid(T).name(), pluginName));
    }

    static PluginStorage* instance();

private:
    void registerPluginClass(const QString& pluginName, const QString& className, std::function<void*()> constructor);
    bool unregisterPluginClass(const QString& pluginName, const QString& className);

    void *createPluginClass(const QString& className, const QString& pluginName);

    PluginStorage();
    ~PluginStorage();

private:
    PluginStoragePrivate* d;
};

}

#endif // GPUI_PLUGINSTORAGE_H
