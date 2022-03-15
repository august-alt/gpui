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

#ifndef GPUI_ICONV_WRAPPER_H
#define GPUI_ICONV_WRAPPER_H

#include <string>

#include <iconv.h>

namespace gpui {

/*!
 * \brief The IconWrapper provides wrapper for POSIX iconv functionality to ease the access from C++
 * and a convenient way to operate on std::string buffers.
 */
class IconvWrapper {
    const iconv_t invalidOpen = (iconv_t) -1;

    IconvWrapper(IconvWrapper const &) = delete;
    IconvWrapper &operator=(IconvWrapper const &) = delete;

    iconv_t conv = nullptr;
    std::string fromEncoding {};
    std::string toEncoding {};

public:
    IconvWrapper(std::string from_encoding, std::string to_encoding);
    ~IconvWrapper();

    /*!
     * \brief Convert std::string to another format.
     */
    std::string convert(std::string from);

private:
    /*!
     * \brief Check if there were conversion errors.
     */
    void checkConversionError();
};

}

#endif // GPUI_ICONV_WRAPPER_H
