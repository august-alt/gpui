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

#include "settings.h"

#include <QMetaProperty>
#include <QSettings>

namespace gpui
{
class SettingsPrivate
{
public:
    QSettings settings{};

    SettingsPrivate(QString directory, QString file)
        : settings(directory, file)
    {}

private:
    SettingsPrivate(const SettingsPrivate &) = delete;            // copy ctor
    SettingsPrivate(SettingsPrivate &&)      = delete;            // move ctor
    SettingsPrivate &operator=(const SettingsPrivate &) = delete; // copy assignment
    SettingsPrivate &operator=(SettingsPrivate &&) = delete;      // move assignment
};

Settings::Settings(QString directory, QString file)
    : d(new SettingsPrivate(directory, file))
{}

Settings::~Settings()
{
    delete d;
}

void Settings::saveSettings(QString section, QObject *snapinSettings)
{
    for (int i = snapinSettings->metaObject()->propertyOffset(); i < snapinSettings->metaObject()->propertyCount(); ++i)
    {
        QMetaProperty currentProperty = snapinSettings->metaObject()->property(i);

        QString propertyName = currentProperty.name();

        QByteArray propName = propertyName.toLocal8Bit().data();

        QString fullPropertyName = QString(section + "/" + propertyName).remove("\\s").trimmed();

        d->settings.setValue(fullPropertyName, QVariant(snapinSettings->property(propName)));
    }
}

void Settings::loadSettings(QString section, QObject *snapinSettings)
{
    for (int i = snapinSettings->metaObject()->propertyOffset(); i < snapinSettings->metaObject()->propertyCount(); ++i)
    {
        QMetaProperty currentProperty = snapinSettings->metaObject()->property(i);

        QString propertyName = currentProperty.name();

        QString fullPropertyName = QString(section + "/" + propertyName).remove("\\s").trimmed();

        QVariant value = d->settings.value(fullPropertyName, QVariant::fromValue(nullptr));

        if (!value.isNull())
        {
            snapinSettings->setProperty(propertyName.toLocal8Bit().data(), value);
        }
    }
}

} // namespace gpui
