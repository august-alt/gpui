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

#ifndef _ISETTINGSWIDGET_H
#define _ISETTINGSWIDGET_H

#include "core.h"
#include "isnapinmanagementsettings.h"

#include <QtWidgets>

namespace gpui
{
class ISettingsWidgetPrivate;

/**
 * @brief Base class for snapin's settings widgets
 */
class GPUI_CORE_EXPORT ISettingsWidget : public QWidget
{
public:
    Q_OBJECT

public:
    virtual ~ISettingsWidget();

    /*!
     * \brief getName Return tab name for settings widget.
     * \return
     */
    virtual QString getName() const = 0;

    /*!
     * \brief saveSettings Saves settings of snapin using settings manager
     */
    virtual void saveSettings() = 0;

    /*!
     * \brief loadSettings Load snapin settings using settings manager
     */
    virtual void loadSettings() = 0;

protected:
    ISettingsWidget(ISnapInManagementSettings *manager, QWidget *parent = nullptr);

    ISnapInManagementSettings *getSettingsManager();

private:
    ISettingsWidgetPrivate *d;

private:
    ISettingsWidget(const ISettingsWidget &) = delete;
    ISettingsWidget(ISettingsWidget &&)      = delete;
    ISettingsWidget &operator=(const ISettingsWidget &) = delete;
    ISettingsWidget &operator=(ISettingsWidget &&) = delete;
};

} // namespace gpui

#endif //_ISETTINGS_WIDGET_H
