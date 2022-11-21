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

#ifndef SCRIPTSSNAPIN_H
#define SCRIPTSSNAPIN_H

#include "../../core/abstractsnapin.h"
#include "../../core/core.h"

#include "../../../src/core/common.h"

#include "scriptssnapinprivate.h"

namespace scripts_plugin
{
class ScriptsSnapInPrivate;

class GPUI_SYMBOL_EXPORT ScriptsSnapIn : public gpui::AbstractSnapIn
{
public:
    ScriptsSnapIn();
    virtual ~ScriptsSnapIn();

    virtual void onInitialize(QMainWindow *mainWindow) override;

    void onShutdown() override;

    virtual void onDataLoad(const std::string &policyPath, const std::string &locale) override;

    virtual void onDataSave() override;

    virtual void onRetranslateUI(const std::string &locale) override;

public:
    ScriptsSnapInPrivate *d;

private:
    ScriptsSnapIn(const ScriptsSnapIn &) = delete;            // copy ctor
    ScriptsSnapIn(ScriptsSnapIn &&)      = delete;            // move ctor
    ScriptsSnapIn &operator=(const ScriptsSnapIn &) = delete; // copy assignment
    ScriptsSnapIn &operator=(ScriptsSnapIn &&) = delete;      // move assignment
};
} // namespace scripts_plugin

#endif // SCRIPTSSNAPIN_H
