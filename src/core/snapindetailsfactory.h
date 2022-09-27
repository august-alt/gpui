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

#ifndef _SNAPINDETAILSFACTORYBASE_H
#define _SNAPINDETAILSFACTORYBASE_H

#include "core.h"

#include "factory.h"
#include "isnapindetailsdialog.h"

namespace gpui
{
/**
 * @brief Base for all for snap-in dialog factories.
 */
class GPUI_CORE_EXPORT SnapInDetailsFactory
{
public:
    typedef ::gpui::Factory<ISnapInDetailsDialog, QString> Factory;
    virtual ~SnapInDetailsFactory() = default;

    static Factory::Type create(Factory::Key const &name) { return factory.create(name); }

    template<class Derived>
    static void define(Factory::Key const &name)
    {
        bool new_key = factory.define(name, &Factory::template create_func<ISnapInDetailsDialog, Derived>);
        if (!new_key)
        {
            throw std::logic_error(std::string(__PRETTY_FUNCTION__) + ": name is already registered!");
        }
    }

private:
    static Factory factory;
};

} // namespace gpui

#define REGISTER_DETAILS_DIALOG_CLASS(cls) static SnapInDetailsFactory myfactory::define<#cls>(new #cls().getType());

#endif //_SNAPINDETAILSFACTORYBASE_H
