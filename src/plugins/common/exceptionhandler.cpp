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

#include "exceptionhandler.h"

#include "policydefinitionfiles.h"

namespace gpui {

bool ExceptionHandler::handleOperation(std::function<void ()> operation, std::function<void (const std::string&)> errorHandler)
{
    try {
        operation();
        return true;
    }
    catch (const xsd::cxx::tree::unexpected_element<char>& e)
    {
        errorHandler("Error an unexpected element: " + e.encountered_name() + " in namespace: "
                     + e.encountered_namespace() + " was encountered. Expected element: "
                     + e.expected_name() + " in namespace: " + e.expected_namespace());
    }
    catch (const xsd::cxx::tree::expected_element<char>& e)
    {
        errorHandler("Error an expected element: " + e.name()
                  + " in namespace: " + e.namespace_() +" was not encountered: ");
    }
    catch (const xsd::cxx::tree::expected_attribute<char>& e)
    {
        errorHandler("Error an expected attribute: " + e.name()
                  + " in namespace: " + e.namespace_() +" was not encountered: ");
    }
    catch (const xsd::cxx::tree::unexpected_enumerator<char>& e)
    {
        errorHandler("Error an unexpected enumerator: " + e.enumerator());
    }
    catch (const xsd::cxx::tree::expected_text_content<char>& e)
    {
        errorHandler(std::string("Error text content was expected for an element.") + e.what());
    }
    catch (const xsd::cxx::tree::no_element_info<char>& e)
    {
        errorHandler("Error no information available for element: " + e.element_name());
    }
    catch (const xsd::cxx::tree::no_type_info<char>& e)
    {
        errorHandler("Error no type information available for: " + e.type_name() + " in namespace: "
                     + e.type_namespace());
    }
    catch (const xsd::cxx::tree::duplicate_id<char>& e)
    {
        errorHandler("Error duplicate ID value was encountered in the object model: " + e.id());
    }
    catch (const xsd::cxx::tree::no_prefix_mapping<char>& e)
    {
        errorHandler("Error no prefix-namespace mapping was provided for prefix: " + e.prefix());
    }
    catch (const xsd::cxx::tree::bounds<char>& e)
    {
        errorHandler(std::string("Size of argument exceeds the capacity argument: ") + e.what());
    }
    catch (const xsd::cxx::tree::exception<char>& e)
    {
        errorHandler(std::string("Verification error: ") + e.what());
    }
    catch (const xsd::cxx::xml::invalid_utf16_string&)
    {
        errorHandler("Invalid UTF-16 text in DOM model.");
    }
    catch (const xsd::cxx::xml::invalid_utf8_string&)
    {
        errorHandler("Invalid UTF-8 text in object model.");
    }
    catch (const std::exception& e)
    {
        errorHandler(std::string("Error: ") + e.what());
    }

    return false;
}

}
