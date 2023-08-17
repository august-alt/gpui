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

#include "sdmlformat.h"

#include "../security/schema/security.h"

#include "../security/model/presentation/securitypresentationresources.h"

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

class XsdCheckBoxAdapter : public security::CheckBox
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::CheckBox CheckBox;

public:
    XsdCheckBoxAdapter(const CheckBox &widget)
        : security::CheckBox()
    {
        this->defaultChecked = widget.defaultChecked();

        assign_if_exists(this->postfix, widget.postfix());
    }

    static std::unique_ptr<security::CheckBox> create(const CheckBox &widget)
    {
        return std::make_unique<XsdCheckBoxAdapter>(widget);
    }
};

class XsdComboBoxAdapter : public security::ComboBox
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::ComboBox ComboBox;

public:
    XsdComboBoxAdapter(const ComboBox &widget)
        : security::ComboBox()
    {
        this->refId = widget.refId();
        assign_if_exists(this->postfix, widget.postfix());

        assign_if_exists(this->defaultValue, widget.defaultChecked());

        for (const auto& suggestionText : widget.suggestion())
        {
            this->suggestion.emplace_back(suggestionText);
        }
    }

    static std::unique_ptr<security::ComboBox> create(const ComboBox &widget)
    {
        return std::make_unique<XsdComboBoxAdapter>(widget);
    }
};

class XsdDecimalTextBoxAdapter : public security::DecimalTextBox
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::DecimalTextBox DecimalTextBox;

public:
    XsdDecimalTextBoxAdapter(const DecimalTextBox &widget)
        : security::DecimalTextBox()
    {
        this->spinBox = widget.spin();
        assign_if_exists(this->defaultValue, widget.defaultValue());
        assign_if_exists(this->spinStep, widget.spinStep());
        assign_if_exists(this->postfix, widget.postfix());
    }

    static std::unique_ptr<security::DecimalTextBox> create(const DecimalTextBox &widget)
    {
        return std::make_unique<XsdDecimalTextBoxAdapter>(widget);
    }
};

class XsdDropdownListAdapter : public security::DropdownList
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::DropdownList DropdownList;

public:
    XsdDropdownListAdapter(const DropdownList &widget)
        : security::DropdownList()
    {
        Q_UNUSED(widget);
        // TODO: Implement.
    }

    static std::unique_ptr<security::DropdownList> create(const DropdownList &widget)
    {
        return std::make_unique<XsdDropdownListAdapter>(widget);
    }
};

class XsdListBoxAdapter : public security::ListBox
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::ListBox ListBox;

public:
    XsdListBoxAdapter(const ListBox &widget)
        : security::ListBox()
    {
        assign_if_exists(this->postfix, widget.postfix());
    }

    static std::unique_ptr<security::ListBox> create(const ListBox &widget)
    {
        return std::make_unique<XsdListBoxAdapter>(widget);
    }
};

class XsdLongDecimalTextBoxAdapter : public security::LongDecimalTextBox
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::LongDecimalTextBox LongDecimalTextBox;

public:
    XsdLongDecimalTextBoxAdapter(const LongDecimalTextBox &widget)
        : security::LongDecimalTextBox()
    {
        this->spinBox = widget.spin();
        assign_if_exists(this->defaultValue, widget.defaultValue());
        assign_if_exists(this->spinStep, widget.spinStep());
        assign_if_exists(this->postfix, widget.postfix());
    }

    static std::unique_ptr<security::LongDecimalTextBox> create(const LongDecimalTextBox &widget)
    {
        return std::make_unique<XsdLongDecimalTextBoxAdapter>(widget);
    }
};

class XsdMultiTextBoxAdapter : public security::MultiTextBox
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::MultiTextBox MultiTextBox;

public:
    XsdMultiTextBoxAdapter(const MultiTextBox &widget)
        : security::MultiTextBox()
    {
//        this->defaultHeight = widget.defaultHeight();
        this->refId         = widget.refId();
//        this->showAsDialog  = widget.showAsDialog();
    }

    static std::unique_ptr<security::MultiTextBox> create(const MultiTextBox &widget)
    {
        return std::make_unique<XsdMultiTextBoxAdapter>(widget);
    }
};

class XsdTextBoxAdapter : public security::TextBox
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::TextBox TextBox;

public:
    XsdTextBoxAdapter(const TextBox &widget)
        : security::TextBox()
    {
        if (widget.defaultValue().present())
        {
            this->defaultValue = widget.defaultValue().get();
        }
        this->label = widget.label();
        this->refId = widget.refId();
    }

    static std::unique_ptr<security::TextBox> create(const TextBox &widget)
    {
        return std::make_unique<XsdTextBoxAdapter>(widget);
    }
};

class XsdTextAdapter : public security::Text
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::TextElement TextElement;

public:
    XsdTextAdapter(const TextElement &widget)
        : security::Text()
    {
        Q_UNUSED(widget);
        // TODO: Implement.
    }

    static std::unique_ptr<security::Text> create(const TextElement &string)
    {
        return std::make_unique<XsdTextAdapter>(string);
    }
};

template<typename AdapterType, typename SequenceType>
void adapt_widgets(const SequenceType &sequence,
                   std::map<std::string, std::shared_ptr<security::DataElement>> &widgets)
{
    for (const auto &adaptee : sequence)
    {
        auto adaptedElement = AdapterType::create(adaptee);

        widgets[adaptee.refId()] = std::move(adaptedElement);
    }
}

class XsdResourcesAdapter : public security::SecurityPresentationResources
{
private:
    typedef ::GroupPolicy::SecurityDefinitions::SecurityPresentationResources SecurityPresentationResources;

public:
    XsdResourcesAdapter(const SecurityPresentationResources &resources)
        : security::SecurityPresentationResources()
    {
        for (const auto &presentation : resources.presentationTable())
        {
            Q_UNUSED(presentation);
            // TODO: Implement.
        }
    }

    static std::shared_ptr<security::SecurityPresentationResources> create(const SecurityPresentationResources &resources)
    {
        return std::make_shared<XsdResourcesAdapter>(resources);
    }
};

SdmlFormat::SdmlFormat()
    : io::PolicyFileFormat<SdmlFile>("sdml")
{
}

bool SdmlFormat::read(std::istream &input, SdmlFile *file)
{
    Q_UNUSED(input);

    std::unique_ptr<::GroupPolicy::SecurityDefinitions::SecurityPresentationResources> securityDefinitions;
    auto operation = [&]() {
        securityDefinitions = GroupPolicy::SecurityDefinitions::SecurityPresentationResources_(input, ::xsd::cxx::tree::flags::dont_validate);

        auto securityPresentation = XsdResourcesAdapter::create(*securityDefinitions);

        file->add(securityPresentation);
    };

    auto errorHandler = [&](const std::string &error) { this->setErrorString(error); };

    return ExceptionHandler::handleOperation(operation, errorHandler);
}

bool SdmlFormat::write(std::ostream &output, SdmlFile *file)
{
    Q_UNUSED(output);
    Q_UNUSED(file);

    return false;
}

}
