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

#ifndef GPUI_ABSTRACT_REGISTRY_ENTRY_H
#define GPUI_ABSTRACT_REGISTRY_ENTRY_H

#include "registryentrytype.h"

#include <QString>

namespace model
{
namespace registry
{
/*!
         * \brief The AbstractRegistryEntry class Represents base of registry entry.
         */
class AbstractRegistryEntry
{
public:
    /*!
     * brief
     */
    RegistryEntryType type = REG_SZ;

    /*!
     * \brief key An identifier of a record used to distinguish the record and efficiently search for it.
     *  The same Key can appear multiple times in the message. HKLM and HKCU MUST NOT be included in the identifier.
     *  If registry.pol file is under the computer-scoped path, the root of the key MUST be HKLM;
     *  if the file is under the user-scoped path, the root of the key MUST be HKCU.
     */
    QString key{};

    /*!
     * \brief value The name of the column in a conceptual database record.
     */
    QString value{};

    virtual ~AbstractRegistryEntry() = default;
};
} // namespace registry
} // namespace model

#endif // GPUI_ABSTRACT_REGISTRY_ENTRY_H
