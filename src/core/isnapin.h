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

#ifndef _ISNAPIN_H
#define _ISNAPIN_H

#include "core.h"
#include "isettingswidget.h"
#include "isnapinsettingsmanager.h"

#include <QAbstractItemModel>
#include <QMainWindow>
#include <QUuid>
#include <QVersionNumber>

namespace gpui
{
/**
 * @brief The ISnapIn interface is a base for all snap-ins in a system.
 */
class GPUI_CORE_EXPORT ISnapIn
{
public:
    virtual ~ISnapIn() = default;

    /**
     * @brief Called when a snap-in is initialized.
     * It allows derived classes to provide additional initialization.
     */
    virtual void onInitialize(QMainWindow *mainWindow) = 0;

    /**
     * @brief onShutdown Called when the snap-in is shutting down.
     * Once this call returns, actions which modify the application, such as adding nodes,
     *  or modifying view descriptions, are not honored.
     */
    virtual void onShutdown() = 0;

    /**
     * @brief onDataLoad Called when snap-in loads data from policy folder.
     * @param policyPath Path to load policy data from.
     * @param locale Locale to use.
     */
    virtual void onDataLoad(const std::string &policyPath, const std::string &locale) = 0;

    /**
     * @brief onDataSave Called when snap-in saves data.
     */
    virtual void onDataSave() = 0;

    /**
     * @brief Gets the identifier for the instance of the snap-in.
     *  The identifier is unique across snap-in instances within a application.
     */
    virtual QUuid getId() const = 0;

    /**
     * @brief getType Returns a type of a snap-in.
     */
    virtual QString getType() const = 0;

    /**
     * @brief Gets the identifier for the instance of the snap-in.
     * The identifier is unique across snap-in instances within a application.
     * This value can never be a null value or invalid.
     */
    virtual QAbstractItemModel *getRootNode() const = 0;

    /**
     * @brief getDisplayName Returns display name of a snap-in.
     */
    virtual QString getDisplayName() const = 0;

    /**
     * @brief getHelpText Returns the help text for the instance of the snap-in.
     * @return
     */
    virtual QString getHelpText() const = 0;

    /**
     * @brief getVersion Returns version of a snap-in.
     * @return
     */
    virtual QVersionNumber getVersion() const = 0;

    /**
     * @brief getLicense Returns license assosiated with the snap-in.
     */
    virtual QString getLicense() const = 0;

    /**
     * @brief getCopyright Returns copyright of snap-in.
     */
    virtual QString getCopyright() const = 0;

    /**
     * @brief onRetranslateUI Retranslate UI
     * @param locale locale to use in current snap-in.
     */
    virtual void onRetranslateUI(const std::string &locale) = 0;

    /*!
     * \brief getSettingsWidget Returns pointer to settings widget.
     * \return Pointer to settings widget.
     */
    virtual ISettingsWidget *getSettingsWidget() const = 0;

    /*!
     * \brief getSettingsManager Returns pointer to settings manager
     * \return Pointer to settings Manager
     */
    virtual ISnapInSettingsManager *getSettingsManager() const = 0;

    /*!
     * \brief setSettingsManaget set SettingsManager for snap-in
     * \param manager
     */
    virtual void setSettingsManager(ISnapInSettingsManager *manager) = 0;
};

} // namespace gpui

#endif //_ISNAPIN_H
