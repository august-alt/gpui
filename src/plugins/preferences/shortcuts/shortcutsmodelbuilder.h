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

#ifndef GPUI_SHORTCUTS_MODEL_BUILDER_H
#define GPUI_SHORTCUTS_MODEL_BUILDER_H

#include <memory>

#include "common/basemodelbuilder.h"
#include "common/preferencesmodel.h"
#include "schemas/shortcutsschema.h"

namespace preferences
{
class ShortcutsModelBuilder : public BaseModelBuilder
{
public:
    ShortcutsModelBuilder();

    std::unique_ptr<PreferencesModel> schemaToModel(std::unique_ptr<Shortcuts> &shortcuts);

    std::unique_ptr<Shortcuts> modelToSchema(std::unique_ptr<PreferencesModel> &model);

private:
    std::string decodeShortcutKey(unsigned char shortcutKey);

    unsigned char encodeShortcutKey(std::string shortcutKey);

    int decodeWindowMode(const std::string &windowMode);

    std::string encodeWindowMode(const int &windowMode);

    std::string encodeLocation(const int targetType);

    int decodeLocation(const std::string &type);
};
} // namespace preferences

#endif // GPUI_SHORTCUTS_MODEL_BUILDER_H
