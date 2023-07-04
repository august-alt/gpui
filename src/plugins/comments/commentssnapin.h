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
#ifndef COMMENTS_SNAP_IN_H
#define COMMENTS_SNAP_IN_H

#include "abstractsnapin.h"

namespace gpui
{
class CommentsSnapInPrivate;

class CommentsSnapIn final : public AbstractSnapIn
{
public:
    CommentsSnapIn();

    void onInitialize(QMainWindow *mainWindow) override;

    void onShutdown() override;

    void onDataLoad(const std::string &policyPath, const std::string &locale) override;

    void onDataSave() override;

    void onRetranslateUI(const std::string &locale) override;

private:
    std::unique_ptr<CommentsSnapInPrivate> d;
};

} // namespace gpui

#endif // COMMENTS_SNAP_IN_H
