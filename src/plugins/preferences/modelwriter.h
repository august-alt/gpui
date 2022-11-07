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

#ifndef GPUI_PREFERENCES_SNAPIN_MODEL_WRITER_H
#define GPUI_PREFERENCES_SNAPIN_MODEL_WRITER_H

#include <memory>
#include <string>

#include "common/preferencesmodel.h"

namespace preferences
{
class ModelWriter
{
public:
    ModelWriter();

    void saveModels(const std::string &policyPath,
                    const std::string &policyType,
                    std::map<std::string, std::unique_ptr<PreferencesModel>> *map);

private:
    ModelWriter(const ModelWriter &) = delete;            // copy ctor
    ModelWriter(ModelWriter &&)      = delete;            // move ctor
    ModelWriter &operator=(const ModelWriter &) = delete; // copy assignment
    ModelWriter &operator=(ModelWriter &&) = delete;      // move assignment

private:
    void createDirectory(const std::string &directoryName);
};

} // namespace preferences

#endif //GPUI_PREFERENCES_SNAPIN_MODEL_WRITER_H
