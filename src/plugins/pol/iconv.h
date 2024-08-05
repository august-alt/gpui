/*
 * libparsepol - POL Registry file parser
 *
 * Copyright (C) 2024 BaseALT Ltd.
 * Copyright (C) 2020 Korney Yakovlevich
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PREGPARSER_ICONV
#define PREGPARSER_ICONV

#include <iconv.h>

namespace pol {
static const size_t ICONV_ERROR_CODE = std::numeric_limits<size_t>::max();
static const iconv_t ICONV_ERROR_DESCRIPTOR = reinterpret_cast<iconv_t>(ICONV_ERROR_CODE);
} // namespace pol

#endif // PREGPARSER_ICONV
