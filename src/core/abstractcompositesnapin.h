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

#ifndef _ABSTRACTCOMPOSITESNAPIN_H
#define _ABSTRACTCOMPOSITESNAPIN_H

#include "core.h"

#include "icompositesnapin.h"

#include "abstractsnapin.h"

namespace gpui
{
class AbstractCompositeSnapInPrivate;

class GPUI_CORE_EXPORT AbstractCompositeSnapIn : public ICompositeSnapIn, public AbstractSnapIn
{
public:
    QMap<QString, QVersionNumber> getDependencies() const override final;

protected:
    AbstractCompositeSnapIn(QString type,
                            QString name,
                            QString helpText       = {},
                            QVersionNumber version = {},
                            QString license        = {},
                            QString copyright      = {});
    ~AbstractCompositeSnapIn();

protected:
    void addDependency(const QString &name, const QVersionNumber &version);

    void removeDependency(const QString &name);

private:
    AbstractCompositeSnapIn(const AbstractCompositeSnapIn &) = delete;            // copy ctor
    AbstractCompositeSnapIn(AbstractCompositeSnapIn &&)      = delete;            // move ctor
    AbstractCompositeSnapIn &operator=(const AbstractCompositeSnapIn &) = delete; // copy assignment
    AbstractCompositeSnapIn &operator=(AbstractCompositeSnapIn &&) = delete;      // move assignment

private:
    AbstractCompositeSnapInPrivate *d;
};

} // namespace gpui

#endif //_ABSTRACTCOMPOSITESNAPIN_H
