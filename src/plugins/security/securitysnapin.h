/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#ifndef GPUI_SECURITY_SNAPIN_H
#define GPUI_SECURITY_SNAPIN_H

#include "core/abstractsnapin.h"

namespace gpui
{

class SecuritySnapInPrivate;

class SecuritySnapIn final : public AbstractSnapIn
{
public:
    SecuritySnapIn();

    void onInitialize(QMainWindow *mainWindow) override;

    void onShutdown() override;

    void onDataLoad(const std::string &policyPath, const std::string &locale) override;

    void onDataSave() override;

    void onRetranslateUI(const std::string &locale) override;

private:
    std::unique_ptr<SecuritySnapInPrivate> d;
};

} // namespace gpui

#endif//GPUI_SECURITY_SNAPIN_H
