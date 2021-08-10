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

#ifndef GPUI_PREG_WRITER_H
#define GPUI_PREG_WRITER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "pregdata.h"

namespace preg {

class PregWriter {
private:
    std::ofstream* preg_file;
    char preg_magic[4] { 'P', 'R', 'e', 'g' };
    char preg_version[4] { '\x01', '\x00', '\x00', '\x00' };

public:
    PregWriter(std::ostream *preg_file);
    ~PregWriter();

    void addEntry(preg::PregEntry &pentry);

    void close();

private:
    void preg_type2buf(uint16_t type);
};

}

#endif // GPUI_PREG_WRITER_H

