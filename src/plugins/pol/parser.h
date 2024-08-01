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
#ifndef PREGPARSER_PARSER
#define PREGPARSER_PARSER

#include <istream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <iconv.h>

namespace pol {

enum class PolicyRegType {
    REG_NONE,
    /* Null-terminated-string */
    REG_SZ = 1,
    REG_EXPAND_SZ = 2,

    /* Any kind of binary data */
    REG_BINARY = 3,
    /* 32-bit number */
    REG_DWORD_LITTLE_ENDIAN = 4,

    /* 32-bit number in NBO format */
    REG_DWORD_BIG_ENDIAN = 5,

    /* A null-terminated Unicode string that contains the target path of a
     * symbolic link. */
    REG_LINK = 6,

    /* Sequence of null-terminated strings terminated by null-terminator */
    REG_MULTI_SZ = 7,
    REG_RESOURCE_LIST = 8,
    REG_FULL_RESOURCE_DESCRIPTOR = 9,
    REG_RESOURCE_REQUIREMENTS_LIST = 10,

    /* 64-bit number */
    REG_QWORD_LITTLE_ENDIAN = 11,
    REG_QWORD_BIG_ENDIAN = 12,
};

typedef std::variant<std::string, std::vector<std::string>, std::vector<uint8_t>, uint32_t,
                     uint64_t>
        PolicyData;

typedef struct PolicyInstruction
{
    inline bool operator==(const PolicyInstruction &other) const
    {
        return type == other.type && data == other.data;
    }
    inline bool operator!=(const PolicyInstruction &other) const
    {
        return type != other.type && data != other.data;
    }

    PolicyRegType type{};
    PolicyData data{};
} PolicyInstruction;

typedef std::unordered_map<std::string, std::unordered_map<std::string, std::vector<PolicyInstruction>>>
        PolicyTree;

typedef struct PolicyBody
{
    inline bool operator==(const PolicyBody &other) const
    {
        return instructions == other.instructions;
    }
    inline bool operator!=(const PolicyBody &other) const
    {
        return instructions != other.instructions;
    }

    PolicyTree instructions{};
} PolicyBody;

typedef struct PolicyFile
{
    inline bool operator==(const PolicyFile &other) const { return body == other.body; }
    inline bool operator!=(const PolicyFile &other) const { return body != other.body; }

    std::optional<PolicyBody> body{};
} PolicyFile;

class PRegParser
{
private:
    /*!
     * \brief Check regex `\x50\x52\x65\x67\x01\x00\x00\x00`
     */
    void parseHeader(std::istream &stream);
    /*!
     * \brief Check regex `(.{4})` and return first group as uint32_t (LE, it will be converted to
     * native)
     */
    uint32_t getSize(std::istream &stream);
    /*!
     * \brief Convert binary data from stream to PolicyData
     */
    PolicyData getData(std::istream &stream, PolicyRegType type, uint32_t size);
    /*!
     * \brief Check 32bit LE regex `([\x1\x2\x3\x4\x5\x6\x7\x8\x9\xA\xB\xC])` and return first
     * group as Type
     */
    PolicyRegType getType(std::istream &stream);
    /*!
     * \brief Matches regex `([\x20-\x5B\x5D-\x7E]\x00)+` and return
     * string as result (UTF-16LE will be converted to UTF-8)
     */
    std::string getKey(std::istream &stream);
    /*!
     * \brief Matches regex
     * `((:?([\x20-\x5B\x5D-\x7E]\x00)+)(:?\x5C\x00([\x20-\x5B\x5D-\x7E]\x00)+)+)` and return first
     * group as result
     */
    std::string getKeypath(std::istream &stream);
    /*!
     * \brief Matches regex `((:?[\x20-\x7E]\x00){1,259})` and return first group as result
     * (UTF-16LE will be converted to UTF-8)
     */
    std::string getValue(std::istream &stream);
    /*!
     * \brief Matches ABNF `LBracket KeyPath SC Value SC Type SC Size SC Data RBracket`,
     * where LBracket `\x5B\x00`, RBracket `\x5D\x00`, SC `\x3B\x00`. Return reduced structure
     */
    void insertInstruction(std::istream &stream, PolicyTree &tree);

    /*!
     * \brief Put `\x50\x52\x65\x67\x01\x00\x00\x00` into stream
     */
    void writeHeader(std::ostream &stream);
    /*!
     * \brief Put instruction, with ABNF
     * `LBracket KeyPath SC Value SC Type SC Size SC Data RBracket`,
     * where LBracket `\x5B\x00`, RBracket `\x5D\x00`, SC `\x3B\x00`, into stream.
     */
    void writeInstruction(std::ostream &stream, const PolicyInstruction &instruction,
                          std::string key, std::string value);

    /*!
     * \brief Put PolicyRegData by PolicyRegType into stringstream
     */
    std::stringstream getDataStream(const PolicyData &data, PolicyRegType type);

public:
    PRegParser();
    virtual PolicyFile parse(std::istream &stream);
    virtual bool write(std::ostream &stream, const PolicyFile &file);
    virtual ~PRegParser();

private:
    PRegParser(const pol::PRegParser &) = delete;
    void operator=(const pol::PRegParser &) = delete;

    ::iconv_t m_iconv_read_id{};
    ::iconv_t m_iconv_write_id{};
};

std::unique_ptr<PRegParser> createPregParser();

} // namespace pol

#endif // PREGPARSER_PARSER
