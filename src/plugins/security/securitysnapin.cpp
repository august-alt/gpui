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

#include "securitysnapin.h"

namespace gpui
{
class SecuritySnapInPrivate
{
public:
};

SecuritySnapIn::SecuritySnapIn()
    : AbstractSnapIn("ISnapIn",
                     "SecuritySnapIn",
                     "Snap-in for security management.",
                     {1, 0, 0},
                     "GPL-2.0",
                     "Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>")
    , d(new SecuritySnapInPrivate())
{
}

void SecuritySnapIn::onInitialize(QMainWindow *mainWindow)
{
    Q_UNUSED(mainWindow);
}

void SecuritySnapIn::onShutdown()
{
}

void SecuritySnapIn::onDataLoad(const std::string &policyPath, const std::string &locale)
{
    Q_UNUSED(policyPath);
    Q_UNUSED(locale);
}

void SecuritySnapIn::onDataSave()
{
}

void SecuritySnapIn::onRetranslateUI(const std::string &locale)
{
    Q_UNUSED(locale);
}

}
