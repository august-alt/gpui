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

#ifndef GPUI_BASE_PREFERENCE_READER_H
#define GPUI_BASE_PREFERENCE_READER_H

#include "interfaces/preferencesreaderinterface.h"

namespace preferences
{
class BasePreferenceReader : public PreferenceReaderInterface
{
public:
    std::string getType() const override final;

    std::unique_ptr<PreferencesModel> read(const std::string path) override final;

protected:
    BasePreferenceReader(const std::string &type);

    virtual std::unique_ptr<PreferencesModel> createModel(std::istream &input) = 0;

private:
    std::string readerType{};
};

} // namespace preferences

#endif //GPUI_BASE_PREFERENCE_READER_H
