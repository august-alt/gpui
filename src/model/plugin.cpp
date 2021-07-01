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

#include "plugin.h"
#include "pluginstorage.h"

#include <QLibrary>

namespace gpui {

    class PluginPrivate
    {
    public:
        QString name;
        std::unique_ptr<QLibrary> library;
        std::map<QString, std::function<void()> > pluginClasses;
    };

    Plugin::~Plugin()
    {
        d->library->unload();

        delete d;
    }

    const QString& Plugin::getName() const
    {
        return d->name;
    }

    void Plugin::setLibrary(std::unique_ptr<QLibrary> library)
    {
        d->library = std::move(library);
    }

    QLibrary* Plugin::getLibrary() const
    {
        return d->library.get();
    }

    const std::map<QString, std::function<void ()> >& Plugin::getPluginClasses() const
    {
        return d->pluginClasses;
    }

    Plugin::Plugin(const QString& name)
        : d(new PluginPrivate())
    {
        d->name = name;
    }

    Plugin::Plugin(const char *name)
        : Plugin(QString(name))
    {

    }

    void Plugin::registerPluginClass(const QString &name, std::function<void ()> constructor)
    {
        d->pluginClasses[name] = constructor;
    }
}
