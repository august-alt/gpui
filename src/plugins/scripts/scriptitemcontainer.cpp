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

#include "scriptitemcontainer.h"
#include "groupscriptcontaineritem.h"
#include "scriptitem.h"

#include <QDebug>

namespace scripts_plugin
{
ScriptItemContainer::ScriptItemContainer()
    : ModelView::CompoundItem("ScriptItemContainer")
{
    addProperty(SECTION_NAME, "");

    addProperty<GroupScriptContainerItem>(SCRIPTS)->setVisible(false);
}

ScriptItemContainer::ScriptItemContainer(const ScriptItemContainer &other)
    : ModelView::CompoundItem()
{
    addProperty(SECTION_NAME, other.property<std::string>(SECTION_NAME));

    addProperty(SCRIPTS, other.SCRIPTS);
}

GroupScriptContainerItem *ScriptItemContainer::getScripts() const
{
    return dynamic_cast<GroupScriptContainerItem *>(children().back());
}

void ScriptItemContainer::retranslateStrings()
{
    qWarning() << "TRanslate section: "
               << this->property<std::string>(ScriptItemContainer::SECTION_NAME).c_str();
}

} // namespace scripts_plugin
