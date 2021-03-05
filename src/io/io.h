/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd.
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

#ifndef GPUI_IO_H
#define GPUI_IO_H

#include "../model/common.h"

#ifdef GPUI_IO_LIBRARY
    #define GPUI_IO_EXPORT GPUI_SYMBOL_EXPORT
#else
    #define GPUI_IO_EXPORT GPUI_SYMBOL_IMPORT
#endif

namespace gpui {

}

#endif // GPUI_IO_H
