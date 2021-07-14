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

#include "admlformat.h"

#include "../common/basetypes.h"
#include "../common/policydefinitions.h"
#include "../common/policydefinitionfiles.h"

#include "../../../src/model/presentation/policyresources.h"

#include "../../../src/model/presentation/checkbox.h"
#include "../../../src/model/presentation/combobox.h"
#include "../../../src/model/presentation/decimaltextbox.h"
#include "../../../src/model/presentation/dropdownlist.h"
#include "../../../src/model/presentation/listbox.h"
#include "../../../src/model/presentation/longdecimaltextbox.h"
#include "../../../src/model/presentation/multitextbox.h"
#include "../../../src/model/presentation/text.h"
#include "../../../src/model/presentation/textbox.h"

#include "../../../src/model/presentation/presentation.h"

#include "../common/exceptionhandler.h"

namespace gpui {

class XsdCheckBoxAdapter : public model::presentation::CheckBox {
private:
    typedef ::GroupPolicy::PolicyDefinitions::CheckBox CheckBox;

public:
    XsdCheckBoxAdapter(const CheckBox& widget)
        : model::presentation::CheckBox(nullptr)
    {
        this->defaultChecked = widget.defaultChecked();
    }

    static std::unique_ptr<model::presentation::CheckBox> create(const CheckBox& widget)
    {
        return std::make_unique<XsdCheckBoxAdapter>(widget);
    }
};

class XsdComboBoxAdapter : public model::presentation::ComboBox {
private:
    typedef ::GroupPolicy::PolicyDefinitions::ComboBox ComboBox;

public:
    XsdComboBoxAdapter(const ComboBox& widget)
        : model::presentation::ComboBox(nullptr)
    {
        if (widget.default_().present())
        {
            this->defaultValue = widget.default_().get();
        }
        this->label = widget.label();
        this->refId = widget.refId();

        for (const auto& suggestion : widget.suggestion())
        {
            this->suggestions.emplace_back(suggestion);
        }
    }

    static std::unique_ptr<model::presentation::ComboBox> create(const ComboBox& widget)
    {
        return std::make_unique<XsdComboBoxAdapter>(widget);
    }
};

class XsdDecimalTextBoxAdapter : public model::presentation::DecimalTextBox {
private:
    typedef ::GroupPolicy::PolicyDefinitions::DecimalTextBox DecimalTextBox;

public:
    XsdDecimalTextBoxAdapter(const DecimalTextBox& widget)
        : model::presentation::DecimalTextBox(nullptr)
    {
        this->defaultValue = widget.defaultValue();
        this->spin = widget.spin();
        this->spinStep = widget.spinStep();
    }

    static std::unique_ptr<model::presentation::DecimalTextBox> create(const DecimalTextBox& widget)
    {
        return std::make_unique<XsdDecimalTextBoxAdapter>(widget);
    }
};

class XsdDropdownListAdapter : public model::presentation::DropdownList {
private:
    typedef ::GroupPolicy::PolicyDefinitions::DropdownList DropdownList;

public:
    XsdDropdownListAdapter(const DropdownList& widget)
        : model::presentation::DropdownList(nullptr)
    {
        if (widget.defaultItem().present())
        {
            this->defaultItem = widget.defaultItem().get();
        }
        this->noSort = widget.noSort();
    }

    static std::unique_ptr<model::presentation::DropdownList> create(const DropdownList& widget)
    {
        return std::make_unique<XsdDropdownListAdapter>(widget);
    }
};

class XsdListBoxAdapter : public model::presentation::ListBox {
private:
    typedef ::GroupPolicy::PolicyDefinitions::ListBox ListBox;

public:
    XsdListBoxAdapter(const ListBox& widget)
        : model::presentation::ListBox(nullptr)
    {
        Q_UNUSED(widget);
    }

    static std::unique_ptr<model::presentation::ListBox> create(const ListBox& widget)
    {
        return std::make_unique<XsdListBoxAdapter>(widget);
    }
};

class XsdLongDecimalTextBoxAdapter : public model::presentation::LongDecimalTextBox {
private:
    typedef ::GroupPolicy::PolicyDefinitions::LongDecimalTextBox LongDecimalTextBox;

public:
    XsdLongDecimalTextBoxAdapter(const LongDecimalTextBox& widget)
        : model::presentation::LongDecimalTextBox(nullptr)
    {
        this->defaultValue = widget.defaultValue();
        this->spin = widget.spin();
        this->spinStep = widget.spinStep();
    }

    static std::unique_ptr<model::presentation::LongDecimalTextBox> create(const LongDecimalTextBox& widget)
    {
        return std::make_unique<XsdLongDecimalTextBoxAdapter>(widget);
    }
};


class XsdMultiTextBoxAdapter : public model::presentation::MultiTextBox {
private:
    typedef ::GroupPolicy::PolicyDefinitions::MultiTextBox MultiTextBox;

public:
    XsdMultiTextBoxAdapter(const MultiTextBox& widget)
        : model::presentation::MultiTextBox(nullptr)
    {
        this->defaultHeight = widget.defaultHeight();
        this->refId = widget.refId();
        this->showAsDialog = widget.showAsDialog();
    }

    static std::unique_ptr<model::presentation::MultiTextBox> create(const MultiTextBox& widget)
    {
        return std::make_unique<XsdMultiTextBoxAdapter>(widget);
    }
};

class XsdTextBoxAdapter : public model::presentation::TextBox {
private:
    typedef ::GroupPolicy::PolicyDefinitions::TextBox TextBox;

public:
    XsdTextBoxAdapter(const TextBox& widget)
        : model::presentation::TextBox(nullptr)
    {
        if (widget.defaultValue().present())
        {
            this->defaultValue = widget.defaultValue().get();
        }
        this->label = widget.label();
        this->refId = widget.refId();
    }

    static std::unique_ptr<model::presentation::TextBox> create(const TextBox& widget)
    {
        return std::make_unique<XsdTextBoxAdapter>(widget);
    }
};

class XsdTextAdapter : public model::presentation::Text {
private:
    typedef ::GroupPolicy::PolicyDefinitions::String TextElement;

public:
    XsdTextAdapter(const TextElement& string)
        : model::presentation::Text(nullptr)
    {
        this->content = string;
    }

    static std::unique_ptr<model::presentation::Text> create(const TextElement& string)
    {
        return std::make_unique<XsdTextAdapter>(string);
    }
};

template <typename AdapterType, typename SequenceType>
void adapt_widgets(const SequenceType& sequence,
                   std::map<std::string, std::shared_ptr<model::presentation::PresentationWidget>>& widgets)
{
    for (const auto& adaptee : sequence) {
        auto adaptedElement = AdapterType::create(adaptee);        

        widgets[adaptee.refId()] = std::move(adaptedElement);
    }
}

template <typename AdapterType, typename SequenceType = ::GroupPolicy::PolicyDefinitions::PolicyPresentation::TextSequence>
void adapt_widgets(const ::GroupPolicy::PolicyDefinitions::PolicyPresentation::TextSequence& sequence,
                   std::map<std::string, std::shared_ptr<model::presentation::PresentationWidget>>& widgets)
{
    for (const auto& adaptee : sequence) {
        auto adaptedElement = AdapterType::create(adaptee);

        widgets[adaptee] = std::move(adaptedElement);
    }
}

class XsdResourcesAdapter : public model::presentation::PolicyResources {
private:
    typedef ::GroupPolicy::PolicyDefinitions::PolicyDefinitionResources PolicyResources;

public:
    XsdResourcesAdapter(const PolicyResources& resources)
        : model::presentation::PolicyResources()
    {
        this->description = resources.description();
        this->displayName = resources.displayName();

        if (resources.resources().stringTable().present())
        {
            for (const auto& string : resources.resources().stringTable()->string())
            {
                this->stringTable[string.id()] = string;
            }
        }

        if (resources.resources().presentationTable().present())
        {
            for (const auto& presentation : resources.resources().presentationTable()->presentation())
            {
                this->presentationTable[presentation.id()] = std::make_shared<model::presentation::Presentation>();
                auto& widgets = this->presentationTable[presentation.id()]->widgets;
                auto& widgetsVector = this->presentationTable[presentation.id()]->widgetsVector;

                const xercesc::DOMNode* n = presentation._node();
                assert (n->getNodeType () == xercesc::DOMNode::ELEMENT_NODE);
                const xercesc::DOMElement* re = static_cast<const xercesc::DOMElement*>(n);

                for (n = re->getFirstChild (); n != 0; n = n->getNextSibling ())
                {
                    if (n->getNodeType () == xercesc::DOMNode::ELEMENT_NODE)
                    {
                        auto elementNode = static_cast<const xercesc::DOMElement*>(n);
                        auto elementType = QString::fromStdU16String(elementNode->getTagName());
                        if (elementType.compare("checkBox") == 0)
                        {
                            auto checkBox = std::make_unique<::GroupPolicy::PolicyDefinitions::CheckBox>(*elementNode);
                            widgets[checkBox->refId()] = XsdCheckBoxAdapter::create(*checkBox);
                            widgetsVector.emplace_back(widgets[checkBox->refId()]);
                        }
                        else if (elementType.compare("comboBox") == 0)
                        {
                            auto comboBox = std::make_unique<::GroupPolicy::PolicyDefinitions::ComboBox>(*elementNode);
                            widgets[comboBox->refId()] = XsdComboBoxAdapter::create(*comboBox);
                            widgetsVector.emplace_back(widgets[comboBox->refId()]);
                        }
                        else if (elementType.compare("decimalTextBox") == 0)
                        {
                            auto decimalTextBox = std::make_unique<::GroupPolicy::PolicyDefinitions::DecimalTextBox>(*elementNode);
                            widgets[decimalTextBox->refId()] = XsdDecimalTextBoxAdapter::create(*decimalTextBox);
                            widgetsVector.emplace_back(widgets[decimalTextBox->refId()]);
                        }
                        else if (elementType.compare("dropdownList") == 0)
                        {
                            auto dropdownList = std::make_unique<::GroupPolicy::PolicyDefinitions::DropdownList>(*elementNode);
                            widgets[dropdownList->refId()] = XsdDropdownListAdapter::create(*dropdownList);
                            widgetsVector.emplace_back(widgets[dropdownList->refId()]);
                        }
                        else if (elementType.compare("listBox") == 0)
                        {
                            auto listBox = std::make_unique<::GroupPolicy::PolicyDefinitions::ListBox>(*elementNode);
                            widgets[listBox->refId()] = XsdListBoxAdapter::create(*listBox);
                            widgetsVector.emplace_back(widgets[listBox->refId()]);
                        }
                        else if (elementType.compare("longDecimalTextBox") == 0)
                        {
                            auto longDecimalTextBox = std::make_unique<::GroupPolicy::PolicyDefinitions::LongDecimalTextBox>(*elementNode);
                            widgets[longDecimalTextBox->refId()] = XsdLongDecimalTextBoxAdapter::create(*longDecimalTextBox);
                            widgetsVector.emplace_back(widgets[longDecimalTextBox->refId()]);
                        }
                        else if (elementType.compare("multiTextBox") == 0)
                        {
                            auto multiTextBox = std::make_unique<::GroupPolicy::PolicyDefinitions::MultiTextBox>(*elementNode);
                            widgets[multiTextBox->refId()] = XsdMultiTextBoxAdapter::create(*multiTextBox);
                            widgetsVector.emplace_back(widgets[multiTextBox->refId()]);
                        }
                        else if (elementType.compare("textBox") == 0)
                        {
                            auto textBox = std::make_unique<::GroupPolicy::PolicyDefinitions::TextBox>(*elementNode);
                            widgets[textBox->refId()] = XsdTextBoxAdapter::create(*textBox);
                            widgetsVector.emplace_back(widgets[textBox->refId()]);
                        }
                        else if (elementType.compare("text") == 0)
                        {
                            auto text = std::make_unique<::GroupPolicy::PolicyDefinitions::String>(*elementNode);
                            widgets[*text] = XsdTextAdapter::create(*text);
                            widgetsVector.emplace_back(widgets[*text]);
                        }
                    }
                }
            }
        }
    }

    static std::unique_ptr<model::presentation::PolicyResources> create(const PolicyResources& resources)
    {
        return std::make_unique<XsdResourcesAdapter>(resources);
    }
};

AdmlFormat::AdmlFormat()
    : io::PolicyFileFormat<io::PolicyResourcesFile>("adml")
{

}

bool AdmlFormat::read(std::istream &input, io::PolicyResourcesFile *file)
{
    std::unique_ptr<::GroupPolicy::PolicyDefinitions::PolicyDefinitions> policyDefinitions;
    auto operation = [&]() {
        std::unique_ptr<::GroupPolicy::PolicyDefinitions::PolicyDefinitionResources> policyDefinitionResources
            = GroupPolicy::PolicyDefinitions::policyDefinitionResources(input,
                                                                        ::xsd::cxx::tree::flags::dont_validate
                                                                        | ::xsd::cxx::tree::flags::keep_dom
                                                                        | ::xsd::cxx::tree::flags::own_dom);

        file->add(XsdResourcesAdapter::create(*policyDefinitionResources));
    };

    auto errorHandler = [&](const std::string& error){
        this->setErrorString(error);
    };

    return ExceptionHandler::handleOperation(operation, errorHandler);
}

bool AdmlFormat::write(std::ostream &output, io::PolicyResourcesFile *file)
{
    Q_UNUSED(output);
    Q_UNUSED(file);

    return false;
}

}
