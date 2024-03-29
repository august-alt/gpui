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

#ifndef GPUI_CMTL_FORMAT_H
#define GPUI_CMTL_FORMAT_H

#include "../../../src/core/common.h"

#include "../../../src/io/policyfileformat.h"
#include "../../../src/io/commentresourcesfile.h"

namespace gpui
{

class GPUI_SYMBOL_EXPORT CmtlFormat : public io::PolicyFileFormat<io::CommentResourcesFile>
{
public:
    CmtlFormat();

    bool read(std::istream &input, io::CommentResourcesFile *file) override;

    bool write(std::ostream &output, io::CommentResourcesFile *file) override;
};

}

#endif//GPUI_CMTL_FORMAT_H
