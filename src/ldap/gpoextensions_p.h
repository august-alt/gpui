// Copyright (C) 2026 BaseALT Ltd.
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef GPUI_GPOEXTENSIONS_P_H
#define GPUI_GPOEXTENSIONS_P_H

#include "ad_object.h"

#include <QMap>
#include <QRegularExpression>
#include <QSet>
#include <QStringList>

#include <algorithm>

namespace ldap
{
namespace detail
{
inline bool mergeGpoExtensions(const QString &existing, const QString &required, QString &merged)
{
    const QString guid = QStringLiteral("\\{[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}\\}");
    const QRegularExpression group(QStringLiteral("\\[((?:%1){2,})\\]").arg(guid));
    QMap<QString, QSet<QString>> extensions;

    for (const QString &list : {existing, required})
    {
        int offset = 0;
        while (offset < list.size())
        {
            const auto match = group.match(list, offset);
            if (!match.hasMatch() || match.capturedStart() != offset)
            {
                return false;
            }

            const QString identifiers = match.captured(1).toUpper();
            auto &tools = extensions[identifiers.left(38)];
            for (int index = 38; index < identifiers.size(); index += 38)
            {
                tools.insert(identifiers.mid(index, 38));
            }
            offset = match.capturedEnd();
        }
    }

    QString result;
    // MS-GPOL 2.2.4 requires ascending, case-insensitive CSE GUID order.
    for (auto extension = extensions.cbegin(); extension != extensions.cend(); ++extension)
    {
        auto tools = extension.value().values();
        std::sort(tools.begin(), tools.end());
        result += QLatin1Char('[') + extension.key() + tools.join(QString()) + QLatin1Char(']');
    }
    merged = result;
    return true;
}

// Keep the publication sequence testable without changing AdInterface's public API.
template<typename Interface>
bool setGpoExtensions(Interface &interface,
                      const QString &dn,
                      const QString &machineExtensions,
                      const QString &userExtensions,
                      int version)
{
    const QString machineAttribute = QStringLiteral("gPCMachineExtensionNames");
    const QString userAttribute = QStringLiteral("gPCUserExtensionNames");
    const auto objects = interface.search(dn, SearchScope_Object, QString(), {machineAttribute, userAttribute});
    if (objects.size() != 1)
    {
        return false;
    }
    const auto object = objects.cbegin().value();
    if (object.get_dn().isEmpty())
    {
        return false;
    }

    const auto value = [&object](const QString &name) {
        for (const QString &attribute : object.attributes())
        {
            if (attribute.compare(name, Qt::CaseInsensitive) == 0)
            {
                return object.get_string(attribute);
            }
        }
        return QString();
    };

    QString machine;
    QString user;
    if (!mergeGpoExtensions(value(machineAttribute), machineExtensions, machine)
        || !mergeGpoExtensions(value(userAttribute), userExtensions, user))
    {
        return false;
    }

    return interface.attribute_replace_string(dn, machineAttribute, machine)
        && interface.attribute_replace_string(dn, userAttribute, user)
        && interface.attribute_replace_int(dn, QStringLiteral("versionNumber"), version);
}
} // namespace detail
} // namespace ldap

#endif // GPUI_GPOEXTENSIONS_P_H
