/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#include <algorithm>

namespace io
{

template <typename TContent>
class BaseFilePrivate
{
public:
    std::vector<std::shared_ptr<TContent>> definitions = {};
};

/*!
 * \class BaseFile BaseFile.h
 * \brief The BaseFile class represents a policy file.
 * \ingroup io
 *
 * The BaseFile class provides a container for definitions.
 */
template <typename TContent>
BaseFile<TContent>::BaseFile()
    : GenericFile<BaseFile, PolicyFileFormat<BaseFile>>()
    , d(new BaseFilePrivate<TContent>())
{}

template <typename TContent>
BaseFile<TContent>::~BaseFile()
{
    delete d;
}

/*!
 * \brief BaseFile<TContent>::addPolicyDefinitions Adds definitions to the file.
 * \param policyDefinitions Policy definitions to add.
 */
template <typename TContent>
void BaseFile<TContent>::add(std::shared_ptr<TContent> definitions)
{
    d->definitions.emplace_back(definitions);
}

/*!
 * \brief BaseFile<TContent>::removePolicyDefinitions Removes policy definitions from the file.
 * \param policyDefinitions Policy definitions to remove.
 */
template <typename TContent>
void BaseFile<TContent>::remove(std::shared_ptr<TContent> definitions)
{
    d->definitions.erase(std::remove_if(d->definitions.begin(),
                                              d->definitions.end(),
                                              [definitions](std::shared_ptr<TContent> currentPolicy) {
                                                  return currentPolicy == definitions;
                                              }));
}

/*!
 * \brief BaseFile<TContent>::getAllPolicyDefinitions Returns all definitions from the file.
 * \return
 */
template <typename TContent>
std::vector<std::shared_ptr<TContent>> BaseFile<TContent>::getAll()
{
    return d->definitions;
}

/*!
 * \brief BaseFile<TContent>::policyDefinitionsCount Return number of definitions in current file.
 * \return Amount of policy definitions in current file.
 */
template <typename TContent>
size_t BaseFile<TContent>::count()
{
    return d->definitions.size();
}

/*!
 * \brief BaseFile<TContent>::getPolicyDefinitions Returns a definition by index.
 * \param index Index of a definitions.
 * \return If definitions are found than returns definitions otherwise retruns null pointer.
 */
template <typename TContent>
std::shared_ptr<TContent> BaseFile<TContent>::get(const size_t index)
{
    if (index < d->definitions.size())
    {
        return d->definitions[index];
    }

    return std::shared_ptr<TContent>(nullptr);
}

} // namespace io
