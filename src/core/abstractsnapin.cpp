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

#include "abstractsnapin.h"

#include <qstandarditemmodel.h>

namespace gpui
{
class AbstractSnapInPrivate
{
public:
    QString name{};
    QUuid id{};
    QAbstractItemModel *rootNode{nullptr};
    QString type{};
    QString helpText{};
    QVersionNumber version{};
    QString license{};
    QString copyright{};
    ISnapInManagementSettings *settingsManager = nullptr;
};

QUuid AbstractSnapIn::getId() const
{
    return d->id;
}

QAbstractItemModel *AbstractSnapIn::getRootNode() const
{
    return d->rootNode;
}

QString AbstractSnapIn::getDisplayName() const
{
    return d->name;
}

QString AbstractSnapIn::getType() const
{
    return d->type;
}

QString AbstractSnapIn::getHelpText() const
{
    return d->helpText;
}

QVersionNumber AbstractSnapIn::getVersion() const
{
    return d->version;
}

QString AbstractSnapIn::getLicense() const
{
    return d->license;
}

QString AbstractSnapIn::getCopyright() const
{
    return d->copyright;
}

ISettingsWidget *AbstractSnapIn::getSettingsWidget() const
{
    return nullptr;
}

ISnapInManagementSettings *AbstractSnapIn::getSettingsManager() const
{
    return d->settingsManager;
}

void AbstractSnapIn::setSettingsManager(ISnapInManagementSettings *manager)
{
    d->settingsManager = manager;
}

AbstractSnapIn::AbstractSnapIn(
    QString type, QString name, QString helpText, QVersionNumber version, QString license, QString copyright)
    : d(new AbstractSnapInPrivate())
{
    d->name      = name;
    d->type      = type;
    d->helpText  = helpText;
    d->version   = version;
    d->license   = license;
    d->copyright = copyright;
}

AbstractSnapIn::~AbstractSnapIn()
{
    delete d;
}

void AbstractSnapIn::setId(QUuid id)
{
    d->id = id;
}

void AbstractSnapIn::setRootNode(QAbstractItemModel *rootNode)
{
    d->rootNode = rootNode;
}

void AbstractSnapIn::setHelpText(QString text)
{
    d->helpText = text;
}

void AbstractSnapIn::setVersion(QVersionNumber version)
{
    d->version = version;
}

void AbstractSnapIn::setLicense(QString license)
{
    d->license = license;
}

void AbstractSnapIn::setCopyright(QString copyright)
{
    d->copyright = copyright;
}

} // namespace gpui
