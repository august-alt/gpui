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

#ifndef SCRIPTITEMCONTAINER_H
#define SCRIPTITEMCONTAINER_H

#include <mvvm/model/compounditem.h>

class ScriptItem;

namespace scripts_plugin
{
class ScriptItem;
class GroupScriptContainerItem;

class ScriptItemContainer : public ModelView::CompoundItem
{
public:
    static inline const std::string SECTION_NAME = "sectionName";

    static inline const std::string SCRIPTS = "scripts";

    ScriptItemContainer();
    ScriptItemContainer(const ScriptItemContainer &other);

    GroupScriptContainerItem *getScripts() const;
};

} // namespace scripts_plugin

Q_DECLARE_METATYPE(scripts_plugin::ScriptItemContainer)

#endif // SCRIPTITEMCONTAINER_H
