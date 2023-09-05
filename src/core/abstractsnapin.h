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

#ifndef _ABSTRACTSNAPIN_H
#define _ABSTRACTSNAPIN_H

#include "core.h"

#include "isnapin.h"
#include "plugin.h"

#include <QAbstractItemModel>
#include <QUuid>
#include <QVersionNumber>

namespace gpui
{
class AbstractSnapInPrivate;

/**
 * @brief Adapts interface of a ISnapIn for a plugin.
 */
class GPUI_CORE_EXPORT AbstractSnapIn : public virtual ISnapIn
{
public:
    QUuid getId() const override;

    QString getType() const override;

    QAbstractItemModel *getRootNode() const override;

    QString getDisplayName() const override;

    QString getHelpText() const override;

    QVersionNumber getVersion() const override;

    QString getLicense() const override;

    QString getCopyright() const override;

    ISettingsWidget *getSettingsWidget() const override;

    ISnapInManagementSettings *getSettingsManager() const override;

    void setSettingsManager(ISnapInManagementSettings *manager) override;

protected:
    AbstractSnapIn(QString type,
                   QString name,
                   QString helpText       = {},
                   QVersionNumber version = {},
                   QString license        = {},
                   QString copyright      = {});
    ~AbstractSnapIn();

protected:
    void setId(QUuid id);

    void setRootNode(QAbstractItemModel *rootNode);

    void setHelpText(QString text);

    void setVersion(QVersionNumber version);

    void setLicense(QString license);

    void setCopyright(QString copyright);

private:
    AbstractSnapIn(const AbstractSnapIn &) = delete;            // copy ctor
    AbstractSnapIn(AbstractSnapIn &&)      = delete;            // move ctor
    AbstractSnapIn &operator=(const AbstractSnapIn &) = delete; // copy assignment
    AbstractSnapIn &operator=(AbstractSnapIn &&) = delete;      // move assignment

private:
    AbstractSnapInPrivate *d;
};

} // namespace gpui

#endif //_ABSTRACTSNAPIN_H
