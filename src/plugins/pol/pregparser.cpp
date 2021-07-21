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

#include "pregparser.h"

#include "iconvwrapper.h"

namespace preg {

namespace {

    bool isRangeStart(char symbol) {
        if ('[' == symbol) {
            return true;
        }
        return false;
    }

    bool isRangeEnd(char symbol) {
        if (']' == symbol) {
            return true;
        }
        return false;
    }

    bool isPregEntrySeparator(char symbol) {
        if (';' == symbol) {
            return true;
        }
        return false;
    }

}

uint16_t buffer2uint16(const char *type_buffer) {
    uint16_t num =
        static_cast<uint16_t>(static_cast<unsigned char>(type_buffer[1]) << 8 |
            static_cast<unsigned char>(type_buffer[0]));
    return num;
}

uint32_t buffer2uint32(const char *type_buffer) {
    uint32_t num =
        static_cast<uint32_t>(static_cast<unsigned char>(type_buffer[3]) << 24 |
            static_cast<unsigned char>(type_buffer[2]) << 16 |
            static_cast<unsigned char>(type_buffer[1]) << 8 |
            static_cast<unsigned char>(type_buffer[0]));
    return num;
}

uint32_t parse_type(const char *type_buffer) {
    return buffer2uint32(type_buffer);
}

PregParser::PregParser(std::istream &input)
    : polfile(&input)
{
    this->loadRegpol();
}

void PregParser::loadRegpol() {
    if (this->polfile->good()) {
        this->polfile->seekg(0, std::ios::end); /* Go to the end of file */
        this->rawFileSize = this->polfile->tellg(); /* Get file length */

        this->polfile->seekg(0, std::ios::beg); /* Set file position to beginning */

        this->readHeader();
        this->readVersion();
    }
}

void PregParser::readHeader() {
    if (this->polfile->good() && 4 < this->rawFileSize) {
        this->polfile->seekg(0, std::ios::beg); /* Set file position to beginning */
        this->polfile->read(this->header, 4); /* Read first 4 bytes */
    }
    this->checkHeader();
}

void PregParser::readVersion() {
    if (this->polfile->good() && 8 < this->rawFileSize) {
        /* Read bytes 4-7 of the file */
        this->polfile->seekg(4, std::ios::beg);
        this->polfile->read(this->version, 4);
    }
    this->checkVersion();
}

void PregParser::checkHeader() {
    if ('P' == this->header[0] && 'R' == this->header[1] &&
        'e' == this->header[2] && 'g' == this->header[3]) {
    } else {
        throw InvalidMagic();
    }
}

void PregParser::checkVersion() {
    if (1 == this->version[0] && 0 == this->version[1] &&
        0 == this->version[2] && 0 == this->version[3]) {
    } else {
        throw InvalidVersion();
    }
}

char PregParser::readByte(size_t absFileStartOffset) {
    char symbol = 0;
    if (absFileStartOffset < this->rawFileSize) {
        this->polfile->seekg(absFileStartOffset, std::ios::beg);
        this->polfile->read(&symbol, 1);
    }
    // FIXME: Else throw exception.
    return symbol;
}

size_t PregParser::seekNextSeparator(size_t absFileStartOffset) {
    size_t end_offset = absFileStartOffset;
    if (absFileStartOffset < this->rawFileSize) {
        char sym_buf;
        for (size_t abs_file_offset = absFileStartOffset;
             abs_file_offset <= this->rawFileSize; abs_file_offset++) {
            sym_buf = this->readByte(abs_file_offset);
            if (isRangeStart(sym_buf) || isPregEntrySeparator(sym_buf) ||
                isRangeEnd(sym_buf) ||
                abs_file_offset == this->rawFileSize) {

                end_offset = abs_file_offset;
                break;
            }
        }
    } else {
        end_offset = this->rawFileSize;
    }
    return end_offset;
}

std::unique_ptr<KeyEntry> PregParser::getNextKeyEntry() {
    auto entry = std::make_unique<KeyEntry>();
    entry->startOffset = this->nextEntryStartOffset;
    entry->endOffset = this->nextEntryStartOffset;

    /* Check if we're not at the end of file */
    if (this->nextEntryStartOffset < this->rawFileSize) {
        char range_init = this->readByte(this->nextEntryStartOffset);

        /* Check that we're at the beginning of the entry we
         * want to parse */
        if (isRangeStart(range_init)) {
            char sym_buf;

            /* Read file byte by byte seeking for the end of entry */
            for (size_t offset = this->nextEntryStartOffset + 1;
                 offset <= this->rawFileSize; offset++) {
                sym_buf = this->readByte(offset);

                /* Build and return the entry if we're found its end */
                if (isRangeEnd(sym_buf)) {
                    entry->endOffset = offset + 2;
                    this->nextEntryStartOffset = offset + 2;
                    return entry;
                }
            }
        }
    } else {
        return nullptr;
    }

    return entry;
}

std::unique_ptr<PregEntry> PregParser::readEntry(KeyEntry kentry) {
    std::unique_ptr<PregEntry> appentry = std::make_unique<PregEntry>();
    std::vector<std::string> results = this->splitEntry(kentry);
    gpui::IconvWrapper iwrapper("UTF-16LE", "UTF-8");

    std::string vn = iwrapper.convert(results.at(0));
    std::string kn = iwrapper.convert(results.at(1));
    appentry->key = std::string(vn, 0, vn.length() - 1);
    appentry->value = std::string(kn, 0, kn.length() - 1);
    appentry->type = parse_type(results.at(2).c_str());
    appentry->size = buffer2uint32(results.at(3).c_str());
    appentry->data = const_cast<char *>(results.at(4).c_str());

    return appentry;
}

std::unique_ptr<PregEntry> PregParser::getNextEntry() {
    auto keyEntry = this->getNextKeyEntry();
    if (keyEntry)
    {
        return this->readEntry(*keyEntry);
    }

    return nullptr;
}

std::string PregParser::stripSquareBraces(KeyEntry kentry) {
    size_t entry_size = (kentry.endOffset - 2) - (kentry.startOffset + 2);
    char *entry_buffer = new char[entry_size];
    this->polfile->seekg((kentry.startOffset + 2));
    this->polfile->read(entry_buffer, entry_size);
    std::string bufstring(entry_buffer, entry_size);
    delete[] entry_buffer;
    return bufstring;
}

std::vector<std::string>
PregParser::splitEntry(KeyEntry kentry) {
    std::string bufstring = this->stripSquareBraces(kentry);
    const char *raw_buffer = bufstring.c_str();
    std::vector<std::string> results;

    size_t offset = 0;
    for (size_t i = 0; i <= bufstring.length(); i++) {
        if (isPregEntrySeparator(raw_buffer[i]) || i == bufstring.length()) {
            size_t split_length = i - offset;
            std::string buf = std::string(bufstring, offset, split_length);
            results.push_back(buf);
            offset = i + 2; // Skip separator
            i++;
        }
    }

    return results;
}

}
