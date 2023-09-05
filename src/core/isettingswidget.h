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

#ifndef _ISETTINGS_WIDGET_H
#define _ISETTINGS_WIDGET_H

#include "core.h"

#include <QtWidgets>

namespace gpui
{
/**
 * @brief Composite snap-in requires list of dependencies to operate.
 */
class GPUI_CORE_EXPORT ISettingsWidget : public QWidget
{
public:
    Q_OBJECT

public:
    /*!
     * \brief getName Return tab name for settings widget.
     * \return
     */
    virtual QString getName() const = 0;

    /*!
     * \brief saveSettings
     */
    virtual void saveSettings() = 0;

protected:
    ISettingsWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {}
};

} // namespace gpui

#endif //_ISETTINGS_WIDGET_H
