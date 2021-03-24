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

#include "admxformat.h"

#include "policydefinitionfiles.h"

#include "../../../src/io/policyfile.h"

#include "../../../src/model/admx/policy.h"
#include "../../../src/model/admx/policyelement.h"

#include "basetypes.h"

using namespace io;

namespace gpui {

AdmxFormat::AdmxFormat()
    : PolicyFileFormat("admx")
{

}

bool AdmxFormat::read(std::istream &input, PolicyFile *file)
{
    try
    {
        std::unique_ptr<::GroupPolicy::PolicyDefinitions::PolicyDefinitions> policyDefinitions
            = GroupPolicy::PolicyDefinitions::policyDefinitions(input, ::xsd::cxx::tree::flags::dont_validate);

        std::shared_ptr<model::admx::Policy> ourPolicy = std::make_shared<model::admx::Policy>();

        if (policyDefinitions->policies().present()) {
            for (const auto& policy : policyDefinitions->policies()->policy()) {
                std::cout << policy.name() << std::endl;
            }
        }

        file->addPolicy(ourPolicy);

        return true;
    }
    catch (const xsd::cxx::tree::exception<char>& e)
    {
        setErrorString(e.what());

        return false;
    }
    catch (const xsd::cxx::xml::invalid_utf16_string&)
    {
        setErrorString("Invalid UTF-16 text in DOM model.");

        return false;
    }
    catch (const xsd::cxx::xml::invalid_utf8_string&)
    {
        setErrorString("invalid UTF-8 text in object model.");

        return false;
    }

    return false;
}

}
