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

#include "administrativetemplatessnapin.h"

#include <iostream>

#include <QAbstractItemModel>
#include <QStandardItemModel>

#include "bundle/policybundle.h"

namespace gpui
{
class AdministrativeTemplatesSnapInPrivate
{
public:
    AdministrativeTemplatesSnapInPrivate() {}

    std::unique_ptr<QStandardItemModel> model = nullptr;

private:
    AdministrativeTemplatesSnapInPrivate(const AdministrativeTemplatesSnapInPrivate &) = delete; // copy ctor
    AdministrativeTemplatesSnapInPrivate(AdministrativeTemplatesSnapInPrivate &&)      = delete; // move ctor
    AdministrativeTemplatesSnapInPrivate &operator=(const AdministrativeTemplatesSnapInPrivate &)
        = delete; // copy assignment
    AdministrativeTemplatesSnapInPrivate &operator=(AdministrativeTemplatesSnapInPrivate &&) = delete; // move assignment
};

AdministrativeTemplatesSnapIn::AdministrativeTemplatesSnapIn()
    : AbstractSnapIn("ISnapIn",
                     "AdministrativeTemplatesSnapIn",
                     "Snap-in for preferences management.",
                     {1, 0, 0},
                     "GPL-2.0",
                     "Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>")
    , d(new AdministrativeTemplatesSnapInPrivate())
{}

void AdministrativeTemplatesSnapIn::onInitialize()
{
    auto bundle = std::make_unique<model::bundle::PolicyBundle>();
    d->model    = bundle->loadFolder("/home/august/Downloads/ADMX/Program Files/Microsoft Group "
                                  "Policy/Windows 10 October 2020 Update (20H2)/PolicyDefinitions/",
                                  "ru-ru");

    setRootNode(static_cast<QAbstractItemModel *>(d->model.get()));

    std::cout << std::string(__PRETTY_FUNCTION__) << std::endl;
}

void AdministrativeTemplatesSnapIn::onShutdown()
{
    std::cout << std::string(__PRETTY_FUNCTION__) << std::endl;
}

void AdministrativeTemplatesSnapIn::onDataLoad(const std::string &policyPath, const std::string &locale)
{
    Q_UNUSED(policyPath);
    Q_UNUSED(locale);
}

void AdministrativeTemplatesSnapIn::onDataSave() {}

} // namespace gpui
