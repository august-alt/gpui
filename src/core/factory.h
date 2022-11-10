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

#ifndef _FACTORY_H
#define _FACTORY_H

#include <map>
#include <memory>
#include <stdexcept>

namespace gpui
{
/**
 * @brief Implementation of abstract factory pattern.
 */
template<typename Interface, typename TKey = std::string>
class Factory
{
public:
    typedef TKey Key;
    typedef std::unique_ptr<Interface> Type;
    typedef Type (*ConcreteFactory)();

    /**
     * @brief define Defines new pair constructor and key if already not present.
     * @param key Key of an element.
     * @param value Factory that constructs the element.
     * @return True on success.
     */
    bool define(Key const &key, ConcreteFactory const &value)
    {
        return storage.insert(typename Storage::value_type(key, value)).second;
    }

    /**
     * @brief create Creates element of given Type.
     * @param key Key of element ot construct.
     * @return Element of given type.
     */
    Type create(Key const &key)
    {
        typename Storage::const_iterator i = storage.find(key);
        if (i == storage.end())
        {
            throw std::invalid_argument(std::string(__PRETTY_FUNCTION__)
                                        + ": key is not registered!");
        }

        return i->second();
    }

    /**
     *  @brief
     */
    template<class Base, class Actual>
    static std::unique_ptr<Base> create_func()
    {
        return std::unique_ptr<Base>(new Actual());
    }

private:
    typedef std::map<Key, ConcreteFactory> Storage;
    Storage storage{};
};

} // namespace gpui

#endif //_FACTORY_H
