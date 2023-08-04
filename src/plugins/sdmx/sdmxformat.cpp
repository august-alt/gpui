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

#include "sdmxformat.h"

#include "../security/schema/security.h"
#include "../security/model/sdmx/securitydefinitions.h"

#include "../common/exceptionhandler.h"

namespace gpui
{

template<typename TInput, typename TOutput>
inline void assign_if_exists(TOutput &output, const TInput &input)
{
    if (input.present())
    {
        output = input.get();
    }
}

template<typename ElementType>
void adapter_base(security::SecurityElement *output, const ElementType &input)
{
    output->refId = input.refId();

    if (input.clientExtension().present())
    {
        output->clientExtension = QUuid(input.clientExtension().get().c_str());
    }

//    assign_if_exists(output->propertyName, input.propertyName());
}

template<typename TInput, typename TOutput>
void decimal_adapter_base(TOutput *output, const TInput &input)
{
    adapter_base(output, input);

//    output->maxValue    = input.maxValue().value();
//    output->minValue    = input.minValue();
    output->required    = input.required();
    output->soft        = input.soft();
    output->storeAsText = input.storeAsText();

//    assign_if_exists(output->propertyName, input.propertyName());
}

template<typename AdapterType, typename SequenceType>
void adapt_elements(const SequenceType &sequence, std::vector<std::unique_ptr<security::SecurityElement>> &elements)
{
    for (const auto &adaptee : sequence)
    {
        auto adaptedElement = AdapterType::create(adaptee);

        elements.push_back(std::move(adaptedElement));
    }
}

class XsdBooleanElementAdapter : public security::BooleanElement
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::BooleanElement BooleanElement;

public:
    XsdBooleanElementAdapter(const BooleanElement &element)
    {
        adapter_base(this, element);

        this->propertyName = element.valueName();
    }

    static std::unique_ptr<security::BooleanElement> create(const BooleanElement &element)
    {
        return std::make_unique<XsdBooleanElementAdapter>(element);
    }
};

class XsdDecimalElementAdapter : public security::DecimalElement
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::DecimalElement DecimalElement;

public:
    XsdDecimalElementAdapter(const DecimalElement &element) { decimal_adapter_base(this, element); }

    static std::unique_ptr<security::DecimalElement> create(const DecimalElement &element)
    {
        return std::make_unique<XsdDecimalElementAdapter>(element);
    }
};

class XsdEnumElementAdapter : public security::EnumerationElement
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::EnumerationElement EnumElement;

public:
    XsdEnumElementAdapter(const EnumElement &element)
    {
        adapter_base(this, element);

        this->propertyName = element.valueName();

        this->required = element.required();

//        for (auto &item : element.)
//        {
//            if (item.value().decimal().present())
//            {
//                this->items.push_back(
//                    std::make_pair(item.displayName(),
//                                   std::make_unique<model::admx::DecimalValue>(item.value().decimal().get().value())));
//            }

//            if (item.value().longDecimal().present())
//            {
//                this->items.push_back(std::make_pair(item.displayName(),
//                                                     std::make_unique<model::admx::LongDecimalValue>(
//                                                         item.value().longDecimal().get().value())));
//            }

//            if (item.value().string().present())
//            {
//                this->items.push_back(
//                    std::make_pair(item.displayName(),
//                                   std::make_unique<model::admx::StringValue>(item.value().string().get())));
//            }
//        }
    }

    static std::unique_ptr<security::EnumerationElement> create(const EnumElement &element)
    {
        return std::make_unique<XsdEnumElementAdapter>(element);
    }
};

class XsdListElementAdapter : public security::ListElement
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::ListElement ListElement;

public:
    XsdListElementAdapter(const ListElement &element)
    {
        adapter_base(this, element);

        assign_if_exists(this->valuePrefix, element.valuePrefix());

        this->additive = element.additive();

        this->expandable = element.expandable();

        this->explicitValue = element.explicitValue();
    }

    static std::unique_ptr<security::ListElement> create(const ListElement &element)
    {
        return std::make_unique<XsdListElementAdapter>(element);
    }
};

class XsdTextElementAdapter : public security::TextElement
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::TextElement TextElement;

public:
    XsdTextElementAdapter(const TextElement &element)
    {
        adapter_base(this, element);

        this->propertyName = element.valueName();

        this->required = element.required();

        this->maxLength = element.maxLength();

//        this->expandable = element.expandable();

        this->soft = element.soft();
    }

    static std::unique_ptr<security::TextElement> create(const TextElement &element)
    {
        return std::make_unique<XsdTextElementAdapter>(element);
    }
};

class XsdLongDecimalElementAdapter : public security::LongDecimalElement
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::LongDecimalElement LongDecimalElement;

public:
    XsdLongDecimalElementAdapter(const LongDecimalElement &element) { decimal_adapter_base(this, element); }

    static std::unique_ptr<security::LongDecimalElement> create(const LongDecimalElement &element)
    {
        return std::make_unique<XsdLongDecimalElementAdapter>(element);
    }
};

class XsdMultiTextElementAdapter : public security::MultiTextElement
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::MultiTextElement MultiTextElement;

public:
    XsdMultiTextElementAdapter(const MultiTextElement &element)
    {
        adapter_base(this, element);

        this->propertyName = element.valueName();
    }

    static std::unique_ptr<security::MultiTextElement> create(const MultiTextElement &element)
    {
        return std::make_unique<XsdMultiTextElementAdapter>(element);
    }
};

class XsdSecurityAdapter : public security::SecurityDefinition
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::SecurityDefinition SecurityDefinition;

public:
    XsdSecurityAdapter(const SecurityDefinition &definition)
        : security::SecurityDefinition()
    {
        this->name        = definition.name();
        this->displayName = definition.displayName();

        assign_if_exists(this->explainText, definition.explainText());

        if (definition.parentCategory().present())
        {
            this->parentCategory = definition.parentCategory()->ref();
        }

        assign_if_exists(this->propertyName, definition.valueName());
        assign_if_exists(this->sectionName, definition.sectionName());
    }

    static std::shared_ptr<security::SecurityDefinition> create(const SecurityDefinition &element)
    {
        return std::make_shared<XsdSecurityAdapter>(element);
    }
};

class XsdSecurityDefinitionsAdapter : public security::SecurityDefinitions
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::SecurityDefinitions SecurityDefinitions;

public:
    XsdSecurityDefinitionsAdapter(const SecurityDefinitions &definitions)
        : security::SecurityDefinitions()
    {
        this->revision = definitions.revision();
        this->schemaVersion = definitions.schemaVersion();

        for (const auto securityElement : definitions.security())
        {
            auto ourSecurity = XsdSecurityAdapter::create(securityElement);

            if (securityElement.elements().present())
            {
                adapt_elements<XsdBooleanElementAdapter>(securityElement.elements()->boolean(), ourSecurity->elements);

                adapt_elements<XsdDecimalElementAdapter>(securityElement.elements()->decimal(), ourSecurity->elements);

                adapt_elements<XsdEnumElementAdapter>(securityElement.elements()->enum_(), ourSecurity->elements);

                adapt_elements<XsdTextElementAdapter>(securityElement.elements()->text(), ourSecurity->elements);

                adapt_elements<XsdListElementAdapter>(securityElement.elements()->list(), ourSecurity->elements);

                adapt_elements<XsdLongDecimalElementAdapter>(securityElement.elements()->longDecimal(), ourSecurity->elements);

                adapt_elements<XsdMultiTextElementAdapter>(securityElement.elements()->multiText(), ourSecurity->elements);
            }

            this->security.push_back(ourSecurity);
        }
    }

    static std::shared_ptr<security::SecurityDefinitions> create(const SecurityDefinitions &element)
    {
        return std::make_shared<XsdSecurityDefinitionsAdapter>(element);
    }
};

SdmxFormat::SdmxFormat()
    : io::PolicyFileFormat<io::PolicyResourcesFile>("sdmx")
{
}

bool SdmxFormat::read(std::istream &input, io::PolicyResourcesFile *file)
{
    std::unique_ptr<::GroupPolicy::SecurityDefinitions::SecurityDefinitions> securityDefinitions;
    auto operation = [&]() {
        securityDefinitions = GroupPolicy::SecurityDefinitions::SecurityDefinitions_(input, ::xsd::cxx::tree::flags::dont_validate);

        auto security = XsdSecurityDefinitionsAdapter::create(*securityDefinitions);

//        file->add(security); TODO: Implement.
    };

    auto errorHandler = [&](const std::string &error) { this->setErrorString(error); };

    return ExceptionHandler::handleOperation(operation, errorHandler);
}

bool SdmxFormat::write(std::ostream &output, io::PolicyResourcesFile *file)
{
    Q_UNUSED(output);
    Q_UNUSED(file);

    return false;
}

}
