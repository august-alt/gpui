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

#include "presentationbuildertest.h"

#include "../../../../src/plugins/administrative_templates/presentation/presentation.h"

#include "../../../../src/plugins/administrative_templates/presentation/checkbox.h"
#include "../../../../src/plugins/administrative_templates/presentation/combobox.h"
#include "../../../../src/plugins/administrative_templates/presentation/decimaltextbox.h"
#include "../../../../src/plugins/administrative_templates/presentation/dropdownlist.h"
#include "../../../../src/plugins/administrative_templates/presentation/listbox.h"
#include "../../../../src/plugins/administrative_templates/presentation/longdecimaltextbox.h"
#include "../../../../src/plugins/administrative_templates/presentation/multitextbox.h"
#include "../../../../src/plugins/administrative_templates/presentation/text.h"
#include "../../../../src/plugins/administrative_templates/presentation/textbox.h"

#include "../../../../src/plugins/administrative_templates/ui/presentationbuilder.h"

#include "../../../../src/plugins/administrative_templates/admx/policy.h"
#include "../../../../src/plugins/administrative_templates/admx/policyelement.h"

#include "../../../../src/plugins/administrative_templates/commands/commandgroup.h"

#include "../../../../src/plugins/administrative_templates/registry/polregistrysource.h"
#include "../../../../src/plugins/administrative_templates/registry/registry.h"

#include <memory>

#include <QVBoxLayout>

namespace tests
{
using namespace model::presentation;

void PresentationBuilderTest::build()
{
    Presentation presentation;

    std::unique_ptr<CheckBox> checkBox(new CheckBox(&presentation));
    checkBox->defaultChecked = true;

    std::unique_ptr<ComboBox> comboBox(new ComboBox(&presentation));
    comboBox->defaultValue = "value";
    comboBox->label        = "label";
    comboBox->refId        = "refId_comboBox";
    comboBox->suggestions  = {"one", "two", "three"};

    std::unique_ptr<DecimalTextBox> decimalTextBox(new DecimalTextBox(&presentation));
    decimalTextBox->defaultValue = 1;
    decimalTextBox->spin         = false;
    decimalTextBox->spinStep     = 0;

    std::unique_ptr<DropdownList> dropdownList(new DropdownList(&presentation));
    dropdownList->defaultItem = 0;
    dropdownList->noSort      = false;

    std::unique_ptr<ListBox> listBox(new ListBox(&presentation));

    std::unique_ptr<LongDecimalTextBox> longDecimalTextBox(new LongDecimalTextBox(&presentation));
    longDecimalTextBox->defaultValue = 2;
    longDecimalTextBox->spin         = true;
    longDecimalTextBox->spinStep     = 2;

    std::unique_ptr<MultiTextBox> multiTextBox(new MultiTextBox(&presentation));
    multiTextBox->defaultHeight = 100;
    multiTextBox->refId         = "refId_multiTextBox";
    multiTextBox->showAsDialog  = false;

    std::unique_ptr<Text> text(new Text(&presentation));
    text->content = "text_content";

    std::unique_ptr<TextBox> textBox(new TextBox(&presentation));
    textBox->defaultValue = "default_textBox_value";
    textBox->label        = "label";
    textBox->refId        = "refId_textBox";

    presentation.widgets["checkBox"]           = (std::move(checkBox));
    presentation.widgets["comboBox"]           = (std::move(comboBox));
    presentation.widgets["decimalTextBox"]     = (std::move(decimalTextBox));
    presentation.widgets["dropdownList"]       = (std::move(dropdownList));
    presentation.widgets["listBox"]            = (std::move(listBox));
    presentation.widgets["longDecimalTextBox"] = (std::move(longDecimalTextBox));
    presentation.widgets["multiTextBox"]       = (std::move(multiTextBox));
    presentation.widgets["text"]               = (std::move(text));
    presentation.widgets["textBox"]            = (std::move(textBox));

    auto policy            = std::make_unique<model::admx::Policy>();
    auto registry          = std::make_shared<model::registry::Registry>();
    auto polRegistrySource = std::make_unique<model::registry::PolRegistrySource>(registry);
}

} // namespace tests

QTEST_MAIN(tests::PresentationBuilderTest)
