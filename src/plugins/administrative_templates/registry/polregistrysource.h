/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd. <org@basealt.ru>
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

#ifndef GPUI_POL_REGISTRY_SOURCE_H
#define GPUI_POL_REGISTRY_SOURCE_H

#include "../administrativetemplates.h"

#include "abstractregistrysource.h"

#include <memory>

namespace model
{
namespace registry
{
class PolRegistrySourcePrivate;
class Registry;

/*!
 * \brief The PolRegistrySource class
 */
class GPUI_ADMINISTRATIVE_TEMPLATES_EXPORT PolRegistrySource : public AbstractRegistrySource
{
public:
    PolRegistrySource(std::shared_ptr<Registry> registry);
    ~PolRegistrySource();

    QVariant getValue(const std::string &key, const std::string &valueName) const override final;
    void setValue(const std::string &key,
                  const std::string &valueName,
                  RegistryEntryType type,
                  const QVariant &data) override final;

    bool isValuePresent(const std::string &key, const std::string &valueName) const override final;

    void markValueForDeletion(const std::string &key, const std::string &valueName) override final;
    bool undeleteValue(const std::string &key, const std::string &valueName) override final;
    bool isValueMarkedForDeletion(const std::string &key, const std::string &valueName) const override final;

    std::vector<std::string> getNonSpecialValueNames(const std::string &key) const override final;
    std::vector<std::string> getValueNames(const std::string &key) const override final;
    void markKeyForDeletion(const std::string &key) override final;

    void clearKey(const std::string &key) override final;
    void clearValue(const std::string &key, const std::string &valueName) override final;

private:
    template<typename T>
    void updateValue(const std::string &key, const std::string &valueName, RegistryEntryType type, const T &data);

    template<typename T>
    void createValue(const std::string &key, const std::string &valueName, RegistryEntryType type, const T &data);

    bool ifValueStartsWith(const std::string &key, const std::string &valueStart) const;

    PolRegistrySourcePrivate *d;

private:
    PolRegistrySource(const PolRegistrySource &) = delete;            // copy ctor
    PolRegistrySource(PolRegistrySource &&)      = delete;            // move ctor
    PolRegistrySource &operator=(const PolRegistrySource &) = delete; // copy assignment
    PolRegistrySource &operator=(PolRegistrySource &&) = delete;      // move assignment
};
} // namespace registry
} // namespace model

#endif // GPUI_POL_REGISTRY_SOURCE_H
