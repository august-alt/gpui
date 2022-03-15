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

#include "pregwriter.h"
#include "iconvwrapper.h"

namespace preg {

PregWriter::PregWriter(std::ostream *initial_preg_file) {
    this->preg_file = initial_preg_file;
    this->preg_file->write(this->preg_magic, 4);
    this->preg_file->write(this->preg_version, 4);
}

PregWriter::~PregWriter() {
}

void PregWriter::addEntry(PregEntry &pentry) {
    char null_terminator[2]{ '\x00', '\x00' };
    char separator[2]{ ';', '\x00' };
    char range_start[2]{ '[', '\x00' };
    char range_end[2]{ ']', '\x00' };

    gpui::IconvWrapper iw("UTF-8", "UTF-16LE");

    std::string keyName = iw.convert(pentry.key);
    std::string valueName = iw.convert(pentry.value);

    const char *key = keyName.c_str();
    size_t key_size = keyName.length() * sizeof(char);

    const char *value = valueName.c_str();
    size_t value_size = valueName.length() * sizeof(char);

    char type[2];
    type[0] = pentry.type & 0xFF;
    type[1] = pentry.type >> 8;

    char size[4];
    size[0] = pentry.size & 0xFF;
    size[1] = pentry.size >> 8;
    size[2] = pentry.size >> 16;
    size[3] = pentry.size >> 24;

    this->preg_file->write(range_start, 2);
    this->preg_file->write(key, key_size);
    this->preg_file->write(null_terminator, 2);
    this->preg_file->write(separator, 2);
    this->preg_file->write(value, value_size);
    this->preg_file->write(null_terminator, 2);
    this->preg_file->write(separator, 2);
    this->preg_file->write(type, 2);
    this->preg_file->write(null_terminator, 2);
    this->preg_file->write(separator, 2);
    this->preg_file->write(size, 4);
    this->preg_file->write(separator, 2);
    this->preg_file->write(pentry.data, pentry.size);
    this->preg_file->write(range_end, 2);
}

}
