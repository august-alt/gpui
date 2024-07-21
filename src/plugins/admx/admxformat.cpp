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

#include "admxformat.h"

#include "../common/policydefinitionfiles.h"

#include "../../../src/io/policydefinitionsfile.h"

#include "../../../src/plugins/administrative_templates/admx/policydefinitions.h"

#include "../../../src/plugins/administrative_templates/admx/policy.h"

#include "../../../src/plugins/administrative_templates/admx/policybooleanelement.h"
#include "../../../src/plugins/administrative_templates/admx/policydecimalelement.h"
#include "../../../src/plugins/administrative_templates/admx/policyelement.h"
#include "../../../src/plugins/administrative_templates/admx/policyenumelement.h"
#include "../../../src/plugins/administrative_templates/admx/policylistelement.h"
#include "../../../src/plugins/administrative_templates/admx/policylongdecimalelement.h"
#include "../../../src/plugins/administrative_templates/admx/policymultitextelement.h"
#include "../../../src/plugins/administrative_templates/admx/policytextelement.h"

#include "../common/basetypes.h"
#include "../common/exceptionhandler.h"

using namespace io;

namespace gpui
{
//================================= Helper Functions ===================================================================

template<typename TInput, typename TOutput>
inline void assign_if_exists(TOutput &output, const TInput &input)
{
    if (input.present())
    {
        output = input.get();
    }
}

template<typename AdapterType, typename SequenceType>
void adapt_elements(const SequenceType &sequence, std::vector<std::unique_ptr<model::admx::PolicyElement>> &elements)
{
    for (const auto &adaptee : sequence)
    {
        auto adaptedElement = AdapterType::create(adaptee);

        elements.push_back(std::move(adaptedElement));
    }
}

//================================= Helper Functions ===================================================================

//================================= Adapters ===========================================================================

template<typename ElementType>
void adapter_base(model::admx::PolicyElement *output, const ElementType &input)
{
    output->id = input.id();

    if (input.clientExtension().present())
    {
        output->clientExtension = QUuid(input.clientExtension().get().c_str());
    }

    assign_if_exists(output->key, input.key());
}

template<typename TInput, typename TOutput>
void decimal_adapter_base(TOutput *output, const TInput &input)
{
    adapter_base(output, input);

    output->maxValue    = input.maxValue();
    output->minValue    = input.minValue();
    output->required    = input.required();
    output->soft        = input.soft();
    output->storeAsText = input.storeAsText();

    assign_if_exists(output->valueName, input.valueName());
}

class XsdRegistryValueAdapter
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::ValueItem::ValueType Value;

public:
    static std::unique_ptr<model::admx::AbstractRegistryValue> create(const Value &element)
    {
        if (element.decimal().present())
        {
            auto value  = std::make_unique<model::admx::RegistryValue<uint32_t>>(element.decimal()->value());
            value->type = model::admx::RegistryValueType::DECIMAL;
            return value;
        }

        if (element.longDecimal().present())
        {
            auto value  = std::make_unique<model::admx::RegistryValue<uint64_t>>(element.longDecimal()->value());
            value->type = model::admx::RegistryValueType::LONG_DECIMAL;
            return value;
        }

        if (element.string().present())
        {
            auto adaptedString = QString::fromStdString(element.string().get());
            auto value         = std::make_unique<model::admx::RegistryValue<QString>>(adaptedString);
            value->type        = model::admx::RegistryValueType::STRING;
            return value;
        }

        if (element.delete_().present())
        {
            auto value  = std::make_unique<model::admx::RegistryValue<int>>(0);
            value->type = model::admx::RegistryValueType::DELETE;
            return value;
        }

        return nullptr;
    }
};

class XsdPolicyItemAdapter : public model::admx::PolicyItem
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::ValueItem PolicyItem;

public:
    XsdPolicyItemAdapter(const PolicyItem &element)
    {
        assign_if_exists(this->key, element.key());

        valueName = element.valueName();

        value = XsdRegistryValueAdapter::create(element.value());
    }

    static std::unique_ptr<model::admx::PolicyItem> create(const PolicyItem &element)
    {
        return std::make_unique<XsdPolicyItemAdapter>(element);
    }
};

class XsdPolicyAdapter : public model::admx::Policy
{
public:
    XsdPolicyAdapter(const ::GroupPolicy::PolicyDefinitions::PolicyDefinition &definition)
        : model::admx::Policy()
    {
        this->name        = definition.name();
        this->displayName = definition.displayName();

        assign_if_exists(this->explainText, definition.explainText());

        this->key = definition.key();

        if (definition.parentCategory().present())
        {
            this->parentCategory = definition.parentCategory()->ref();
        }

        assign_if_exists(this->valueName, definition.valueName());

        for (const auto &seeAlsoDefinition : definition.seeAlso())
        {
            this->seeAlso.emplace_back(seeAlsoDefinition);
        }

        switch (definition.class_())
        {
        case ::GroupPolicy::PolicyDefinitions::PolicyClass::Machine:
            this->policyType = model::admx::PolicyType::Machine;
            break;
        case ::GroupPolicy::PolicyDefinitions::PolicyClass::User:
            this->policyType = model::admx::PolicyType::User;
            break;
        case ::GroupPolicy::PolicyDefinitions::PolicyClass::Both:
            this->policyType = model::admx::PolicyType::Both;
            break;
        default:
            break;
        }

        if (definition.presentation().present())
        {
            this->presentation = std::make_unique<std::string>(definition.presentation().get());
        }

        this->supportedOn = definition.supportedOn().ref();

        if (definition.enabledList().present())
        {
            for (auto item : definition.enabledList()->item())
            {
                this->enabledList.push_back(XsdPolicyItemAdapter::create(item));
            }
        }

        if (definition.disabledList().present())
        {
            for (auto item : definition.disabledList()->item())
            {
                this->disabledList.push_back(XsdPolicyItemAdapter::create(item));
            }
        }

        if (definition.enabledValue().present())
        {
            this->enabledValue = XsdRegistryValueAdapter::create(definition.enabledValue().get());
        }

        if (definition.disabledValue().present())
        {
            this->disabledValue = XsdRegistryValueAdapter::create(definition.disabledValue().get());
        }
    }

    static std::shared_ptr<model::admx::Policy> create(const ::GroupPolicy::PolicyDefinitions::PolicyDefinition &input)
    {
        return std::make_shared<XsdPolicyAdapter>(input);
    }
};

class XsdBooleanElementAdapter : public model::admx::PolicyBoolElement
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::BooleanElement BooleanElement;
    typedef ::GroupPolicy::PolicyDefinitions::Value XsdValue;
    typedef ::GroupPolicy::PolicyDefinitions::ValueList XsdList;
    typedef ::model::admx::BooleanValue AdmxBooleanValue;
    typedef ::model::admx::PolicyBoolElement::BooleanList AdmxBooleanList;

    static AdmxBooleanValue XsdValueToAdmxBooleanValue(const XsdValue &value)
    {
        if (value.decimal().present()) {
            return { value.decimal().get().value() };
        }
        if (value.longDecimal().present()) {
            return { value.longDecimal().get().value() };
        }
        if (value.string().present()) {
            return { value.string().get() };
        }
        if (value.delete_().present()) {
            return {};
        }
        throw std::runtime_error("empty XsdValue");
    }

    static BooleanList XsdListToAdmxBooleanList(const XsdList &list)
    {
        AdmxBooleanList target;
        auto &sourceList = list.item();

        target.reserve(sourceList.size());

        for (auto value : sourceList) {
            auto admxValue = XsdValueToAdmxBooleanValue(value.value());
            admxValue.setValueName(value.valueName());

            if (value.key().present()) {
                admxValue.setKey(value.key().get());
            }

            target.emplace_back();
        }
    }

public:
    XsdBooleanElementAdapter(const BooleanElement &element)
    {
        adapter_base(this, element);

        assign_if_exists(this->valueName, element.valueName());

        if (element.trueList().present()) {
            this->hasTrueList = true;
            this->trueList = std::move(XsdListToAdmxBooleanList(element.trueList().get()));
        }

        if (element.falseList().present()) {
            this->hasFalseList = true;
            this->falseList = std::move(XsdListToAdmxBooleanList(element.falseList().get()));
        }
    }

    static std::unique_ptr<model::admx::PolicyBoolElement> create(const BooleanElement &element)
    {
        return std::make_unique<XsdBooleanElementAdapter>(element);
    }
};

class XsdDecimalElementAdapter : public model::admx::PolicyDecimalElement
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::DecimalElement DecimalElement;

public:
    XsdDecimalElementAdapter(const DecimalElement &element) { decimal_adapter_base(this, element); }

    static std::unique_ptr<model::admx::PolicyDecimalElement> create(const DecimalElement &element)
    {
        return std::make_unique<XsdDecimalElementAdapter>(element);
    }
};

class XsdEnumElementAdapter : public model::admx::PolicyEnumElement
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::EnumerationElement EnumElement;

public:
    XsdEnumElementAdapter(const EnumElement &element)
    {
        adapter_base(this, element);

        assign_if_exists(this->valueName, element.valueName());

        this->required = element.required();

        for (auto &item : element.item())
        {
            if (item.value().decimal().present())
            {
                this->items.push_back(
                    std::make_pair(item.displayName(),
                                   std::make_unique<model::admx::DecimalValue>(item.value().decimal().get().value())));
            }

            if (item.value().longDecimal().present())
            {
                this->items.push_back(std::make_pair(item.displayName(),
                                                     std::make_unique<model::admx::LongDecimalValue>(
                                                         item.value().longDecimal().get().value())));
            }

            if (item.value().string().present())
            {
                this->items.push_back(
                    std::make_pair(item.displayName(),
                                   std::make_unique<model::admx::StringValue>(item.value().string().get())));
            }
        }
    }

    static std::unique_ptr<model::admx::PolicyEnumElement> create(const EnumElement &element)
    {
        return std::make_unique<XsdEnumElementAdapter>(element);
    }
};

class XsdListElementAdapter : public model::admx::PolicyListElement
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::ListElement ListElement;

public:
    XsdListElementAdapter(const ListElement &element)
    {
        adapter_base(this, element);

        assign_if_exists(this->valuePrefix, element.valuePrefix());

        this->additive = element.additive();

        this->expandable = element.expandable();

        this->explicitValue = element.explicitValue();
    }

    static std::unique_ptr<model::admx::PolicyListElement> create(const ListElement &element)
    {
        return std::make_unique<XsdListElementAdapter>(element);
    }
};

class XsdTextElementAdapter : public model::admx::PolicyTextElement
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::TextElement TextElement;

public:
    XsdTextElementAdapter(const TextElement &element)
    {
        adapter_base(this, element);

        assign_if_exists(this->valueName, element.valueName());

        this->required = element.required();

        this->maxLength = element.maxLength();

        this->expandable = element.expandable();

        this->soft = element.soft();
    }

    static std::unique_ptr<model::admx::PolicyTextElement> create(const TextElement &element)
    {
        return std::make_unique<XsdTextElementAdapter>(element);
    }
};

class XsdLongDecimalElementAdapter : public model::admx::PolicyLongDecimalElement
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::LongDecimalElement LongDecimalElement;

public:
    XsdLongDecimalElementAdapter(const LongDecimalElement &element) { decimal_adapter_base(this, element); }

    static std::unique_ptr<model::admx::PolicyLongDecimalElement> create(const LongDecimalElement &element)
    {
        return std::make_unique<XsdLongDecimalElementAdapter>(element);
    }
};

class XsdMultiTextElementAdapter : public model::admx::PolicyMultiTextElement
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::MultiTextElement MultiTextElement;

public:
    XsdMultiTextElementAdapter(const MultiTextElement &element)
    {
        adapter_base(this, element);

        assign_if_exists(this->valueName, element.valueName());
    }

    static std::unique_ptr<model::admx::PolicyMultiTextElement> create(const MultiTextElement &element)
    {
        return std::make_unique<XsdMultiTextElementAdapter>(element);
    }
};

class XsdCategoryAdapter : public model::admx::Category
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::Category Category;

public:
    XsdCategoryAdapter(const ::GroupPolicy::PolicyDefinitions::Category &category)
        : model::admx::Category()
    {
        this->displayName = category.displayName();
        assign_if_exists(this->explainText, category.explainText());
        this->name = category.name();

        if (category.parentCategory().present())
        {
            this->parentCategory = category.parentCategory()->ref();
        }

        for (const auto &also : category.seeAlso())
        {
            this->seeAlso.push_back(also);
        }
    }

    static std::shared_ptr<model::admx::Category> create(const Category &element)
    {
        return std::make_shared<XsdCategoryAdapter>(element);
    }
};

class XsdSupportedDefinitionAdapter : public model::admx::SupportedDefinition
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::SupportedOnDefinition SupportedOnDefinition;

public:
    XsdSupportedDefinitionAdapter(const SupportedOnDefinition &category)
        : model::admx::SupportedDefinition()
    {
        this->displayName = category.displayName();
        this->name        = category.name();

        const uint32_t defaultMinVersion = 0;
        const uint32_t defaultMaxVersion = UINT32_MAX;
        if (category.or_().present())
        {
            for (auto &rangeElement : category.or_()->range())
            {
                uint32_t minVersion = defaultMinVersion;
                uint32_t maxVersion = defaultMaxVersion;
                assign_if_exists(minVersion, rangeElement.minVersionIndex());
                assign_if_exists(maxVersion, rangeElement.maxVersionIndex());

                this->or_.emplace_back(rangeElement.ref(), minVersion, maxVersion);
            }

            for (auto &rangeElement : category.or_()->reference())
            {
                this->or_.emplace_back(rangeElement.ref(), defaultMinVersion, defaultMaxVersion);
            }
        }

        if (category.and_().present())
        {
            for (auto &rangeElement : category.and_()->range())
            {
                uint32_t minVersion = defaultMinVersion;
                uint32_t maxVersion = defaultMaxVersion;
                assign_if_exists(minVersion, rangeElement.minVersionIndex());
                assign_if_exists(maxVersion, rangeElement.maxVersionIndex());

                this->and_.emplace_back(rangeElement.ref(), minVersion, maxVersion);
            }

            for (auto &rangeElement : category.and_()->reference())
            {
                this->and_.emplace_back(rangeElement.ref(), defaultMinVersion, defaultMaxVersion);
            }
        }
    }

    static std::shared_ptr<model::admx::SupportedDefinition> create(const SupportedOnDefinition &element)
    {
        return std::make_shared<XsdSupportedDefinitionAdapter>(element);
    }
};

class XsdSupportedProductAdapter : public model::admx::SupportedProduct
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::SupportedProduct SupportedProduct;

public:
    XsdSupportedProductAdapter(const SupportedProduct &category)
        : model::admx::SupportedProduct()
    {
        this->displayName = category.displayName();
        this->name        = category.name();

        for (auto &catregoryMajorVersion : category.majorVersion())
        {
            model::admx::MajorVersion currentMajorVersion;

            currentMajorVersion.name         = catregoryMajorVersion.name();
            currentMajorVersion.displayName  = catregoryMajorVersion.displayName();
            currentMajorVersion.versionIndex = catregoryMajorVersion.versionIndex();

            for (auto &categoryMinorVersion : catregoryMajorVersion.minorVersion())
            {
                model::admx::MinorVersion currentMinorVersion;

                currentMinorVersion.name         = categoryMinorVersion.name();
                currentMinorVersion.displayName  = categoryMinorVersion.displayName();
                currentMinorVersion.versionIndex = categoryMinorVersion.versionIndex();

                currentMajorVersion.minorVersion.push_back(currentMinorVersion);
            }

            this->majorVersion.push_back(currentMajorVersion);
        }
    }

    static std::shared_ptr<model::admx::SupportedProduct> create(const SupportedProduct &element)
    {
        return std::make_shared<XsdSupportedProductAdapter>(element);
    }
};

class XsdPolicyDefinitionsAdapter : public model::admx::PolicyDefinitions
{
private:
    typedef ::GroupPolicy::PolicyDefinitions::PolicyDefinitions PolicyDefinitions;

public:
    XsdPolicyDefinitionsAdapter(const PolicyDefinitions &definitions)
        : model::admx::PolicyDefinitions()
    {
        this->revision      = definitions.revision();
        this->schemaVersion = definitions.schemaVersion();

        auto& target = definitions.policyNamespaces().target();

        this->policyNamespaces.target.prefix = target.prefix();
        this->policyNamespaces.target.namespace_ = target.namespace_();

        for (const auto& using_ : definitions.policyNamespaces().using_())
        {
            this->policyNamespaces.using_.emplace_back(using_.prefix(), using_.namespace_());
        }

        for (const auto &adm : definitions.supersededAdm())
        {
            this->supersededAdm.push_back(adm.fileName());
        }

        this->resources.minRequiredRevision = definitions.resources().minRequiredRevision();
        this->resources.fallbackCulture     = definitions.resources().fallbackCulture();

        if (definitions.supportedOn().present())
        {
            this->supportedOn = std::make_shared<::model::admx::SupportedOn>();

            if (definitions.supportedOn()->definitions().present())
            {
                for (const auto &definition : definitions.supportedOn()->definitions()->definition())
                {
                    this->supportedOn->definitions.push_back(XsdSupportedDefinitionAdapter::create(definition));
                }
            }

            if (definitions.supportedOn()->products().present())
            {
                for (const auto &product : definitions.supportedOn()->products()->product())
                {
                    this->supportedOn->products.push_back(XsdSupportedProductAdapter::create(product));
                }
            }
        }

        if (definitions.categories().present())
        {
            for (const auto &category : definitions.categories()->category())
            {
                this->categories.push_back(XsdCategoryAdapter::create(category));
            }
        }

        if (definitions.policies().present())
        {
            for (const auto &policy : definitions.policies()->policy())
            {
                auto ourPolicy = XsdPolicyAdapter::create(policy);

                if (policy.elements().present())
                {
                    adapt_elements<XsdBooleanElementAdapter>(policy.elements()->boolean(), ourPolicy->elements);

                    adapt_elements<XsdDecimalElementAdapter>(policy.elements()->decimal(), ourPolicy->elements);

                    adapt_elements<XsdEnumElementAdapter>(policy.elements()->enum_(), ourPolicy->elements);

                    adapt_elements<XsdTextElementAdapter>(policy.elements()->text(), ourPolicy->elements);

                    adapt_elements<XsdListElementAdapter>(policy.elements()->list(), ourPolicy->elements);

                    adapt_elements<XsdLongDecimalElementAdapter>(policy.elements()->longDecimal(), ourPolicy->elements);

                    adapt_elements<XsdMultiTextElementAdapter>(policy.elements()->multiText(), ourPolicy->elements);
                }

                this->policies.push_back(ourPolicy);
            }
        }
    }

    static std::shared_ptr<model::admx::PolicyDefinitions> create(const PolicyDefinitions &element)
    {
        return std::make_shared<XsdPolicyDefinitionsAdapter>(element);
    }
};

//================================= Adapters ===========================================================================

//================================= Operators ==========================================================================

std::ostream &operator<<(std::ostream &os, const model::admx::Policy &policy)
{
    os << "Name: " << policy.name << std::endl;
    os << "Display Name: " << policy.displayName << std::endl;
    os << "Key: " << policy.key << std::endl;
    os << "Value: " << policy.valueName << std::endl;
    os << "Explain: " << policy.explainText << std::endl;
    os << "Parent: " << policy.parentCategory << std::endl;

    for (const auto &tag : policy.seeAlso)
    {
        os << "See Also: " << tag << std::endl;
    }

    os << "Policy Type: "
       << (policy.policyType == model::admx::PolicyType::Machine ? "Machine"
           : policy.policyType == model::admx::PolicyType::User  ? "User"
                                                                 : "Both")
       << std::endl;

    os << std::endl;

    return os;
}

inline void element_operator_base(std::ostream &os, const model::admx::PolicyElement &element, const std::string &type)
{
    os << "\t" << type << ":" << std::endl;
    os << "\t\tID: " << element.id << std::endl;
    os << "\t\tClient Extension: " << element.clientExtension.toString().toStdString() << std::endl;
    os << "\t\tKey: " << element.key << std::endl;
}

template<typename TElement>
inline void element_with_value_base(std::ostream &os, const TElement &element, const std::string &type)
{
    element_operator_base(os, element, type);
    os << "\t\tValue: " << element.valueName << std::endl;
}

template<typename TElement>
void decimal_element_operator_base(std::ostream &os, const TElement &element, const std::string &type)
{
    element_with_value_base(os, element, type);

    os << "\t\tMax Value:" << element.maxValue << std::endl;
    os << "\t\tMin Value:" << element.minValue << std::endl;
    os << "\t\tRequired:" << element.required << std::endl;
    os << "\t\tSoft:" << element.soft << std::endl;
    os << "\t\tStore As Text:" << element.storeAsText << std::endl;

    os << std::endl;
}

std::ostream &operator<<(std::ostream &os, const model::admx::PolicyBoolElement &element)
{
    element_with_value_base(os, element, "BooleanElement");
    os << std::endl;

    return os;
}

std::ostream &operator<<(std::ostream &os, const model::admx::PolicyDecimalElement &element)
{
    decimal_element_operator_base(os, element, "DecimalElement");

    return os;
}

std::ostream &operator<<(std::ostream &os, const model::admx::PolicyEnumElement &element)
{
    element_with_value_base(os, element, "EnumElement");
    os << std::endl;

    return os;
}

std::ostream &operator<<(std::ostream &os, const model::admx::PolicyListElement &element)
{
    element_operator_base(os, element, "ListElement");

    os << "\t\tAdditive:" << element.additive << std::endl;
    os << "\t\tExpandable:" << element.expandable << std::endl;
    os << "\t\tExplicit Value:" << element.explicitValue << std::endl;

    os << std::endl;

    return os;
}

std::ostream &operator<<(std::ostream &os, const model::admx::PolicyTextElement &element)
{
    element_with_value_base(os, element, "TextElement");

    os << "\t\tExpandable:" << element.expandable << std::endl;
    os << "\t\tMax Length:" << element.maxLength << std::endl;
    os << "\t\tRequired:" << element.required << std::endl;
    os << "\t\tSoft:" << element.soft << std::endl;

    os << std::endl;

    return os;
}

std::ostream &operator<<(std::ostream &os, const model::admx::PolicyLongDecimalElement &element)
{
    decimal_element_operator_base(os, element, "LongDecimalElement");

    return os;
}

std::ostream &operator<<(std::ostream &os, const model::admx::PolicyMultiTextElement &element)
{
    element_with_value_base(os, element, "MultiTextElement");
    os << std::endl;

    return os;
}

//================================= Operators ==========================================================================

AdmxFormat::AdmxFormat()
    : PolicyFileFormat("admx")
{}

bool AdmxFormat::read(std::istream &input, PolicyDefinitionsFile *file)
{
    std::unique_ptr<::GroupPolicy::PolicyDefinitions::PolicyDefinitions> policyDefinitions;
    auto operation = [&]() {
        policyDefinitions = GroupPolicy::PolicyDefinitions::policyDefinitions(input,
                                                                              ::xsd::cxx::tree::flags::dont_validate);

        file->addPolicyDefinitions(XsdPolicyDefinitionsAdapter::create(*policyDefinitions));
    };

    auto errorHandler = [&](const std::string &error) { this->setErrorString(error); };

    return ExceptionHandler::handleOperation(operation, errorHandler);
}

bool AdmxFormat::write(std::ostream &output, PolicyDefinitionsFile *file)
{
    Q_UNUSED(output);
    Q_UNUSED(file);

    return false;
}

} // namespace gpui
