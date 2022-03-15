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

#ifndef GPUI_PREG_PARSER_H
#define GPUI_PREG_PARSER_H

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "pregdata.h"

namespace preg {

    /*!
     * \brief key_entry This structure contains offsets for PReg file pointing to '[' and ']'
     * characters. It is internal to preg_parser.
     */
    struct KeyEntry {
        size_t startOffset;
        size_t endOffset;
    };

    uint16_t buffer2uint16(const char *type_buffer);
    uint32_t buffer2uint32(const char *type_buffer);
    uint32_t parse_type(const char *type_buffer);

    class PregParser {
    private:
        std::istream* polfile = nullptr;

        std::string filePath = "";

        size_t rawFileSize = 0;

        char header[4] { 'P', 'R', 'e', 'g' };

        char version[4] { '\x01', '\x00', '\x00', '\x00' };

        size_t nextEntryStartOffset = 8;

    public:
        explicit PregParser(std::istream& input);

        std::unique_ptr<PregEntry> getNextEntry();

    protected:
        void loadRegpol();

        void readHeader();
        void readVersion();

        void checkHeader();
        void checkVersion();

        char readByte(size_t absFileStartOffset);

        size_t seekNextSeparator(size_t absFileStartOffset);

        std::string stripSquareBraces(KeyEntry kentry);

        std::unique_ptr<KeyEntry> getNextKeyEntry();

        std::unique_ptr<PregEntry> readEntry(KeyEntry kentry);
        std::vector<std::string> splitEntry(KeyEntry kentry);

    private:
        PregParser(const PregParser&)            = delete;   // copy ctor
        PregParser(PregParser&&)                 = delete;   // move ctor
        PregParser& operator=(const PregParser&) = delete;   // copy assignment
        PregParser& operator=(PregParser&&)      = delete;   // move assignment
    };

}

#endif // GPUI_PREG_PARSER_H
