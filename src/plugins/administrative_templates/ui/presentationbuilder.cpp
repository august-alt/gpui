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

#include "presentationbuilder.h"

#include "../plugins/administrative_templates/presentation/presentation.h"
#include "../plugins/administrative_templates/presentation/presentationwidget.h"
#include "../plugins/administrative_templates/presentation/presentationwidgetvisitor.h"

#include "../plugins/administrative_templates/presentation/checkbox.h"
#include "../plugins/administrative_templates/presentation/combobox.h"
#include "../plugins/administrative_templates/presentation/decimaltextbox.h"
#include "../plugins/administrative_templates/presentation/dropdownlist.h"
#include "../plugins/administrative_templates/presentation/listbox.h"
#include "../plugins/administrative_templates/presentation/longdecimaltextbox.h"
#include "../plugins/administrative_templates/presentation/multitextbox.h"
#include "../plugins/administrative_templates/presentation/text.h"
#include "../plugins/administrative_templates/presentation/textbox.h"

#include "../plugins/administrative_templates/admx/policy.h"
#include "../plugins/administrative_templates/admx/policyelement.h"
#include "../plugins/administrative_templates/admx/policyenumelement.h"

#include "../plugins/administrative_templates/commands/command.h"
#include "../plugins/administrative_templates/commands/commandgroup.h"

#include "../plugins/administrative_templates/registry/abstractregistrysource.h"

#include "../plugins/administrative_templates/admx/policydecimalelement.h"
#include "../plugins/administrative_templates/admx/policylistelement.h"
#include "../plugins/administrative_templates/admx/policylongdecimalelement.h"

#include "../gui/listboxdialog.h"

#include "../gui/altspinbox.h"

#include <QVBoxLayout>

#include <QCheckBox>
#include <QComboBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QTextEdit>

#include <QDebug>

#include <iostream>
#include <queue>

using namespace model::presentation;
using namespace model::admx;
using namespace model::registry;
using namespace model::command;

namespace gpui
{
template<typename TLayoutItem>
QLayoutItem *createAndAttachLabel(QWidget *buddy, const QString &text)
{
    QLabel *label = new QLabel(text.trimmed());
    label->setBuddy(buddy);
    label->setWordWrap(true);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(1);

    label->setSizePolicy(sizePolicy);
    buddy->setSizePolicy(sizePolicy);

    TLayoutItem *container = new TLayoutItem();
    container->addWidget(label);
    container->addWidget(buddy);

    return container;
}

QHBoxLayout *createCaptions()
{
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    QLabel *descriptionLabel      = new QLabel(QObject::tr("Description:"));
    QLabel *optionLabel           = new QLabel(QObject::tr("Options:"));
    horizontalLayout->addWidget(descriptionLabel);
    horizontalLayout->addWidget(optionLabel);
    return horizontalLayout;
}

bool *m_dataChanged  = nullptr;
bool *m_stateEnabled = nullptr;

class PresentationBuilderPrivate : public PresentationWidgetVisitor
{
public:
    struct ElementInfo
    {
        std::string key = "";
        std::string value = "";
        RegistryEntryType type = RegistryEntryType::REG_NONE;
        PolicyElement *element = nullptr;
    };

public:
    virtual bool visit(CheckBox &widget) const override
    {
        bool ans = true;

        QCheckBox *checkBox = new QCheckBox();

        checkBox->setChecked(widget.defaultChecked);
        std::string label;
        if (widget.label.empty())
        {
            if (!m_labels.empty())
            {
                label = m_labels.front();
                ans = false;
            }else{
                label = "";
            }
        }else{
            label = widget.label;
        }
        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(checkBox, QString::fromStdString(label));

        if (m_policy && m_source)
        {
            const ElementInfo elementInfo = findElementInfo();

            if (m_source->isValuePresent(elementInfo.key, elementInfo.value))
            {
                checkBox->setChecked(m_source->getValue(elementInfo.key, elementInfo.value).value<bool>());
            }

            checkBox->connect(checkBox, &QCheckBox::toggled, []() { *m_dataChanged = true; });

            // TODO: Implement correct type on save.
            m_saveButton->connect(m_saveButton, &QPushButton::clicked, [elementInfo, checkBox, this]() {
                if (!(*m_stateEnabled))
                {
                    return;
                }
                if (elementInfo.element->required && !checkBox->isChecked()){
                    return;
                }
                qWarning() << "Presentation builder::save: " << elementInfo.key.c_str() << " "
                           << elementInfo.value.c_str();
                int checked = checkBox->checkState() == Qt::Checked ? 1 : 0;
                m_source->setValue(elementInfo.key, elementInfo.value, RegistryEntryType::REG_DWORD, checked);
            });
        }

        addToLayout(container);

        return ans;
    }

    virtual bool visit(ComboBox &widget) const override
    {
        bool ans = true;
        QComboBox *comboBox = new QComboBox();
        comboBox->setCurrentText(QString::fromStdString(widget.defaultValue));
        for (const auto &item : widget.suggestions)
        {
            comboBox->addItem(QString::fromStdString(item));
        }

        if (m_policy && m_source)
        {
            const ElementInfo elementInfo = findElementInfo();

            if (m_source->isValuePresent(elementInfo.key, elementInfo.value))
            {
                int index = getComboData(m_source->getValue(elementInfo.key, elementInfo.value), elementInfo.element);
                comboBox->setCurrentIndex(index);
            }

            comboBox->connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=]() {
                *m_dataChanged = true;
            });

            // TODO: Implement correct type on save.
            m_saveButton->connect(m_saveButton, &QPushButton::clicked, [elementInfo, comboBox, this]() {
                if (!(*m_stateEnabled))
                {
                    return;
                }                
                qWarning() << "Presentation builder::save: " << elementInfo.key.c_str() << " "
                           << elementInfo.value.c_str();
                setComboData(elementInfo.key, elementInfo.value, elementInfo.type, comboBox, elementInfo.element);
            });
        }

        std::string label;
        if (widget.label.empty())
        {
            if (!m_labels.empty())
            {
                label = m_labels.front();
                ans = false;
            }else{
                label = "";
            }
        }else{
            label = widget.label;
        }
        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(comboBox, QString::fromStdString(label));

        addToLayout(container);

        return ans;
    }

    virtual bool visit(DecimalTextBox &widget) const override
    {
        bool ans = true;
        QWidget *textBox = createAnyDecimalTextBox(widget.spin, widget.defaultValue, widget.spinStep);

        std::string label;
        if (widget.label.empty())
        {
            if (!m_labels.empty())
            {
                label = m_labels.front();
                ans = false;
            }else{
                label = "";
            }
        }else{
            label = widget.label;
        }
        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(textBox, QString::fromStdString(label));

        addToLayout(container);

        return ans;
    }

    virtual bool visit(DropdownList &widget) const override
    {
        bool ans = true;

        QComboBox *comboBox = new QComboBox();
        comboBox->setCurrentIndex(widget.defaultItem);

        std::string label;
        if (widget.label.empty())
        {
            if (!m_labels.empty())
            {
                label = m_labels.front();
                ans = false;
            }else{
                label = "";
            }
        }else{
            label = widget.label;
        }
        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(comboBox, QString::fromStdString(label));

        if (widget.values.size() > 0)
        {
            for (auto &value : widget.values)
            {
                comboBox->addItem(QString::fromStdString(value));
            }
            if (widget.defaultItem < widget.values.size())
            {
                comboBox->setCurrentIndex(widget.defaultItem);
            }
        }

        if (m_policy && m_source)
        {
            const ElementInfo elementInfo = findElementInfo();

            if (m_source->isValuePresent(elementInfo.key, elementInfo.value))
            {
                int index = getComboData(m_source->getValue(elementInfo.key, elementInfo.value), elementInfo.element);
                comboBox->setCurrentIndex(index);
            }

            comboBox->connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=]() {
                *m_dataChanged = true;
            });

            // TODO: Implement correct type on save.
            m_saveButton->connect(m_saveButton, &QPushButton::clicked, [elementInfo, comboBox, this]() {
                if (!(*m_stateEnabled))
                {
                    return;
                }
                qWarning() << "Presentation builder::save: " << elementInfo.key.c_str() << " "
                           << elementInfo.value.c_str();
                setComboData(elementInfo.key, elementInfo.value, elementInfo.type, comboBox, elementInfo.element);
            });
        }

        addToLayout(container);

        return ans;
    }

    virtual bool visit(ListBox &widget) const override
    {
        bool ans = true;

        QPushButton *button = new QPushButton(QObject::tr("Edit"));

        std::string label;
        if (widget.label.empty())
        {
            if (!m_labels.empty())
            {
                label = m_labels.front();
                ans = false;
            }else{
                label = "";
            }
        }else{
            label = widget.label;
        }
        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(button, QString::fromStdString(label));

        PolicyListElement* listElement = nullptr;

        ElementInfo elementInfo;

        if (m_policy && m_source)
        {
             elementInfo = findElementInfo();

            if (listElement = dynamic_cast<PolicyListElement *>(elementInfo.element))
            {
                qWarning() << "List element: " << listElement->id.c_str() << "\n"
                           << "key: " << listElement->key.c_str() << "\n"
                           << "valueName: " << listElement->valueName.c_str() << "\n"
                           << "valuePrefix: " << listElement->valuePrefix.c_str() << "\n"
                           << "additive: " << listElement->additive << "\n"
                           << "expandable: " << listElement->expandable << "\n"
                           << "explicitValue" << listElement->explicitValue << "\n";
            }
            else
            {
                qWarning() << "Unsupported policy element type.";
            }
        }

        auto onClicked = [&, elementInfo, listElement]() {
            gpui::ListBoxDialog *listBox = new gpui::ListBoxDialog(QString::fromStdString(widget.label));
            listBox->setAttribute(Qt::WA_DeleteOnClose);

            if (!listElement || elementInfo.type == REG_NONE)
            {
                qWarning() << "Unable to get valid policy listElement or elementInfo is not initialized.";
                return;
            }

            RegistryEntryType registryEntryType = listElement->expandable ? RegistryEntryType::REG_EXPAND_SZ
                                                                          : RegistryEntryType::REG_SZ;

            if (listElement->explicitValue)
            {
                {
                    QMap<std::string, QString> items;
                    // Create two column widget.
                    auto valueNames = m_source->getValueNames(listElement->key);
                    for (const auto &valueName : valueNames)
                    {
                        items[valueName] = m_source->getValue(listElement->key, valueName).toString();
                    }
                    listBox->setItems(items);
                }

                listBox->connect(listBox,
                                 &gpui::ListBoxDialog::itemsEditingFinished,
                                 [=](QMap<std::string, QString> currentItems) {
                    if (!(*m_stateEnabled))
                    {
                        return;
                    }

                    if (elementInfo.element->required && currentItems.empty()){
                        return;
                    }

                    qWarning() << "Items debug: " << currentItems.values();
                    // clean-up registry values.
                    auto valueNames = m_source->getValueNames(listElement->key);
                    for (const auto &valueName : valueNames)
                    {
                        m_source->clearValue(listElement->key, valueName);
                    }
                    // set-up current values.
                    for (const auto &valueName : currentItems.keys())
                    {
                        auto value = currentItems.value(valueName);
                        if (!value.trimmed().isEmpty())
                        {
                            m_source->setValue(elementInfo.key,
                                               valueName,
                                               registryEntryType,
                                               value);
                        }
                    }
                    *m_dataChanged = true;
                });
            }
            else
            {
                // Create one column widget.
                QStringList items;

                if (listElement->valuePrefix.size() > 0)
                {
                    // If there is a prefix then use prefix to load values.
                    {
                        auto valueNames = m_source->getValueNames(listElement->key);
                        size_t index    = 1;
                        auto valueName  = listElement->valuePrefix + std::to_string(index);
                        while (m_source->isValuePresent(listElement->key, valueName))
                        {
                            items.append(m_source->getValue(listElement->key, valueName).toString());
                            valueName = listElement->valuePrefix + std::to_string(++index);
                        }
                    }

                    listBox->connect(listBox,
                                     &gpui::ListBoxDialog::itemsEditingFinished,
                                     [=](QMap<std::string, QString> currentItems) {
                        if (!(*m_stateEnabled))
                        {
                            return;
                        }

                        if (elementInfo.element->required && currentItems.empty()){
                            return;
                        }

                        qWarning() << "Items debug: " << currentItems.values();
                        size_t index = 1;
                        // clean-up registry values.
                        auto registryValueName = listElement->valuePrefix
                                + std::to_string(index);
                        while (m_source->isValuePresent(listElement->key, registryValueName))
                        {
                            m_source->clearValue(listElement->key, registryValueName);
                            registryValueName = listElement->valuePrefix
                                    + std::to_string(++index);
                        }
                        // set-up current values.
                        for (const auto &item : currentItems.values())
                        {
                            if (!item.trimmed().isEmpty())
                            {
                                auto valueName = listElement->valueName
                                        + std::to_string(index);
                                m_source->setValue(elementInfo.key,
                                                   valueName,
                                                   registryEntryType,
                                                   item);
                            }
                        }
                        *m_dataChanged = true;
                    });
                }
                else
                {
                    auto valueNames = m_source->getValueNames(listElement->key);
                    for (const auto &valueName : valueNames)
                    {
                        items.append(m_source->getValue(listElement->key, valueName).toString());
                    }

                    listBox->connect(listBox,
                                     &gpui::ListBoxDialog::itemsEditingFinished,
                                     [=](QMap<std::string, QString> currentItems) {
                        if (!(*m_stateEnabled))
                        {
                            return;
                        }

                        if (elementInfo.element->required && currentItems.empty()){
                            return;
                        }

                        qWarning() << "Items debug: " << currentItems.values();
                        // clean-up registry values.
                        auto registryValueNames = m_source->getValueNames(listElement->key);
                        for (const auto &valueName : registryValueNames)
                        {
                            m_source->clearValue(listElement->key, valueName);
                        }
                        // set-up current values.
                        for (const auto &item : currentItems.values())
                        {
                            if (!item.trimmed().isEmpty())
                            {
                                m_source->setValue(elementInfo.key,
                                                   item.toStdString(),
                                                   registryEntryType,
                                                   item);
                            }
                        }
                        *m_dataChanged = true;
                    });
                }

                qWarning() << "Items debug: " << items;
                listBox->setItems(items);
            }

            listBox->show();
        };

        QObject::connect(button, &QPushButton::clicked, onClicked);

        addToLayout(container);

        return ans;
    }

    virtual bool visit(LongDecimalTextBox &widget) const override
    {
        bool ans = true;
        QWidget *textBox = createAnyDecimalTextBox(widget.spin, widget.defaultValue, widget.spinStep);

        std::string label;
        if (widget.label.empty())
        {
            if (!m_labels.empty())
            {
                label = m_labels.front();
                ans = false;
            }else{
                label = "";
            }
        }else{
            label = widget.label;
        }
        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(textBox, QString::fromStdString(label));

        addToLayout(container);

        return ans;
    }

    virtual bool visit(MultiTextBox &widget) const override
    {
        bool ans = true;
        QTextEdit *textEdit = new QTextEdit();
        textEdit->setMaximumHeight(widget.defaultHeight * textEdit->fontMetrics().height());

        if (m_policy && m_source)
        {
            const ElementInfo elementInfo = findElementInfo();

            if (m_source->isValuePresent(elementInfo.key, elementInfo.value))
            {
                auto value = m_source->getValue(elementInfo.key, elementInfo.value).value<QString>();
                textEdit->setPlainText(value);
            }

            textEdit->connect(textEdit, &QTextEdit::textChanged, [=]() { *m_dataChanged = true; });

            // TODO: Implement correct type on save.
            m_saveButton->connect(m_saveButton, &QPushButton::clicked, [elementInfo, textEdit, this]() {
                if (!(*m_stateEnabled))
                {
                    return;
                }

                if (elementInfo.element->required && textEdit->toPlainText().isEmpty()){
                    return;
                }

                qWarning() << "Presentation builder::save: " << elementInfo.key.c_str() << " "
                           << elementInfo.value.c_str();
                QStringList data(textEdit->toPlainText());
                m_source->setValue(elementInfo.key, elementInfo.value, RegistryEntryType::REG_MULTI_SZ, data);
            });
        }

        addToLayout(textEdit);

        return ans;
    }

    virtual bool visit(Text &widget) const override
    {
//        QLabel *label = new QLabel();
//        label->setText(QString::fromStdString(widget.content));
//        label->setWordWrap(true);
//        label->setAlignment(Qt::AlignHCenter);
//        addToLayout(label);
        return true;
    }

    virtual bool visit(TextBox &widget) const override
    {
        bool ans = true;

        QLineEdit *lineEdit = new QLineEdit();
        lineEdit->setText(QString::fromStdString(widget.defaultValue));

        if (m_policy && m_source)
        {
            const ElementInfo elementInfo = findElementInfo();

            if (m_source->isValuePresent(elementInfo.key, elementInfo.value))
            {
                auto value = m_source->getValue(elementInfo.key, elementInfo.value).value<QString>();
                lineEdit->setText(value);
            }

            lineEdit->connect(lineEdit, &QLineEdit::textChanged, [=]() { *m_dataChanged = true; });

            // TODO: Implement correct type on save.
            m_saveButton->connect(m_saveButton, &QPushButton::clicked, [elementInfo, lineEdit, this]() {
                if (!(*m_stateEnabled))
                {
                    return;
                }

                if (elementInfo.element->required && lineEdit->text().isEmpty()){
                    return;
                }

                qWarning() << "Presentation builder::save: " << elementInfo.key.c_str() << " "
                           << elementInfo.value.c_str();
                QString data(lineEdit->text());
                m_source->setValue(elementInfo.key,
                                   elementInfo.value,
                                   RegistryEntryType::REG_SZ,
                                   QVariant::fromValue(data));
            });
        }

        std::string label;
        if (widget.label.empty())
        {
            if (!m_labels.empty())
            {
                label = m_labels.front();
                ans = false;
            }else{
                label = "";
            }
        }else{
            label = widget.label;
        }
        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(lineEdit, QString::fromStdString(label));

        addToLayout(container);

        return ans;
    }

    virtual std::string check(CheckBox &widget) const override{return "";}
    virtual std::string check(ComboBox &widget) const override{return "";}
    virtual std::string check(DecimalTextBox &widget) const override{return "";}
    virtual std::string check(DropdownList &widget) const override{return "";}
    virtual std::string check(ListBox &widget) const override{return "";}
    virtual std::string check(LongDecimalTextBox &widget) const override{return "";}
    virtual std::string check(MultiTextBox &widget) const override{return "";}
    virtual std::string check(TextBox &widget) const override{return "";}
    virtual std::string check(Text &widget) const override
    {
        return widget.content;
    }

    void setLayout(QLayout *layout) { m_layout = layout; }

    void setPolicy(const Policy &policy) { m_policy = &policy; }

    void setRegistrySource(AbstractRegistrySource &source) { m_source = &source; }

    void setCurrentElementName(std::string elementName) { m_elementName = elementName; }

    void setSaveButton(QPushButton &saveButton) { m_saveButton = &saveButton; }

    void setDataChanged(bool &dataChanged) { m_dataChanged = &dataChanged; }

    void setStateEnabled(bool &stateEnabled) { m_stateEnabled = &stateEnabled; }

    void addLabel(std::string label){ m_labels.push(label); }

    void popLabel(){ m_labels.pop(); }

private:
    QLayout *m_layout                = nullptr;
    const Policy *m_policy           = nullptr;
    AbstractRegistrySource *m_source = nullptr;
    QPushButton *m_saveButton        = nullptr;
    std::string m_elementName        = "";
    std::queue<std::string> m_labels = std::queue<std::string>();

    void addToLayout(QWidget *widget) const
    {
        if (m_layout)
        {
            m_layout->addWidget(widget);
        }
    }

    void addToLayout(QLayoutItem *container) const
    {
        if (container)
        {
            m_layout->addItem(container);
        }
    }

    template<typename Number>
    QWidget *createAnyDecimalTextBox(bool spin, Number value, Number step) const
    {
        if (spin)
        {
            ::gui::AltSpinBox *spinBox = new ::gui::AltSpinBox();
            spinBox->setMinimum(0);
            spinBox->setMaximum(std::numeric_limits<int>::max());
            spinBox->setSingleStep(step);
            spinBox->setValue(value);

            if (m_policy && m_source)
            {
                const ElementInfo elementInfo = findElementInfo();

                if (m_source->isValuePresent(elementInfo.key, elementInfo.value))
                {
                    spinBox->setValue(m_source->getValue(elementInfo.key, elementInfo.value).value<Number>());
                }

                if (auto decimal = dynamic_cast<PolicyDecimalElement *>(elementInfo.element))
                {
                    spinBox->setMinimum(decimal->minValue);
                    spinBox->setMaximum(decimal->maxValue);
                }

                if (auto longDecimal = dynamic_cast<PolicyLongDecimalElement *>(elementInfo.element))
                {
                    spinBox->setMinimum(longDecimal->minValue);
                    spinBox->setMaximum(longDecimal->maxValue);
                }

                spinBox->connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=]() {
                    *m_dataChanged = true;
                });

                // TODO: Implement correct type on save.
                m_saveButton->connect(m_saveButton, &QPushButton::clicked, [elementInfo, spinBox, this]() {
                    if (!(*m_stateEnabled))
                    {
                        return;
                    }
                    qWarning() << "Presentation builder::save: " << elementInfo.key.c_str() << " "
                               << elementInfo.value.c_str();
                    m_source->setValue(elementInfo.key, elementInfo.value, elementInfo.type, spinBox->value());
                });
            }

            return spinBox;
        }

        QLineEdit *edit = new QLineEdit();
        edit->setText(QString::number(value));
        edit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max()));

        if (m_policy && m_source)
        {
            const ElementInfo elementInfo = findElementInfo();

            if (auto decimal = dynamic_cast<PolicyDecimalElement *>(elementInfo.element))
            {
                edit->setValidator(new QIntValidator(decimal->minValue, decimal->maxValue));
            }

            if (auto longDecimal = dynamic_cast<PolicyLongDecimalElement *>(elementInfo.element))
            {
                edit->setValidator(new QIntValidator(longDecimal->minValue, longDecimal->maxValue));
            }

            if (m_source->isValuePresent(elementInfo.key, elementInfo.value))
            {
                edit->setText(QString(m_source->getValue(elementInfo.key, elementInfo.value).value<Number>()));
            }

            edit->connect(edit, &QLineEdit::textChanged, [=]() { *m_dataChanged = true; });

            // TODO: Implement correct type on save.
            m_saveButton->connect(m_saveButton, &QPushButton::clicked, [elementInfo, edit, this]() {
                if (!(*m_stateEnabled))
                {
                    return;
                }
                qWarning() << "Presentation builder::save: " << elementInfo.key.c_str() << " "
                           << elementInfo.value.c_str();
                m_source->setValue(elementInfo.key, elementInfo.value, elementInfo.type, edit->text().toUInt());
            });
        }

        return edit;
    }

    ElementInfo findElementInfo() const
    {
        if (m_policy && m_source)
        {
            for (const auto &element : m_policy->elements)
            {
                if (element->id.compare(m_elementName) == 0)
                {
                    if (element->key.size() > 0)
                    {
                        return {element->key, element->valueName, element->getRegistryEntryType(), element.get()};
                    }
                    else
                    {
                        return {m_policy->key, element->valueName, element->getRegistryEntryType(), element.get()};
                    }
                }
            }
        }

        qWarning() << "Key and value not found!" << m_elementName.c_str();

        return {"", "", static_cast<RegistryEntryType>(0), nullptr};
    }

    int getComboData(QVariant value, PolicyElement *element) const
    {
        PolicyEnumElement *enumElement = dynamic_cast<PolicyEnumElement *>(element);
        if (enumElement)
        {
            int index = 0;
            for (const auto &it : enumElement->items)
            {
                auto sv = dynamic_cast<StringValue *>(it.second.get());
                if (sv)
                {
                    if (sv->value.compare(value.toString().toStdString()) == 0)
                    {
                        return index;
                    }
                }
                auto dv = dynamic_cast<DecimalValue *>(it.second.get());
                if (dv)
                {
                    if (dv->value == value.toUInt())
                    {
                        return index;
                    }
                }
                auto qv = dynamic_cast<LongDecimalValue *>(it.second.get());
                if (qv)
                {
                    if (qv->value == value.toULongLong())
                    {
                        return index;
                    }
                }
                index++;
            }
        }
        return 0;
    }

    void setComboData(const std::string &key,
                      const std::string &valueName,
                      RegistryEntryType type,
                      QComboBox const *comboBox,
                      PolicyElement *element) const
    {
        std::string stringValue;
        uint32_t dwordValue            = 0;
        uint64_t qwordValue            = 0;
        PolicyEnumElement *enumElement = dynamic_cast<PolicyEnumElement *>(element);
        if (enumElement)
        {
            auto begin = enumElement->items.begin();
            auto it    = std::next(begin, comboBox->currentIndex());
            if (it != enumElement->items.end())
            {
                auto sv = dynamic_cast<StringValue *>(it->second.get());
                if (sv)
                {
                    stringValue = sv->value;
                }
                auto dv = dynamic_cast<DecimalValue *>(it->second.get());
                if (dv)
                {
                    dwordValue = dv->value;
                }
                auto qv = dynamic_cast<LongDecimalValue *>(it->second.get());
                if (qv)
                {
                    qwordValue = qv->value;
                }
                qWarning() << "Element: " << it->first.c_str() << " : " << stringValue.c_str() << " : " << dwordValue
                           << " : " << qwordValue;
            }
        }

        switch (type)
        {
        case REG_SZ:
        case REG_BINARY:
        case REG_EXPAND_SZ:
            m_source->setValue(key, valueName, type, QString::fromStdString(stringValue));
            break;
        case REG_DWORD:
        case REG_DWORD_BIG_ENDIAN:
            m_source->setValue(key, valueName, type, dwordValue);
            break;
        case REG_QWORD:
            m_source->setValue(key, valueName, type, QVariant::fromValue(qwordValue));
            break;
        case REG_MULTI_SZ: {
            m_source->setValue(key, valueName, type, QStringList(QString::fromStdString(stringValue)));
        }
        break;
        default:
            qWarning() << "Unable to detect value type for element with key: " << key.c_str()
                       << " value: " << valueName.c_str();
            break;
        }
    }
};

PresentationBuilderPrivate *gpui::PresentationBuilder::d = new PresentationBuilderPrivate();

QVBoxLayout *::gpui::PresentationBuilder::build(const ::gpui::PresentationBuilderParams &params)
{
    QVBoxLayout *layout = new QVBoxLayout();
    d->setLayout(layout);
    d->setPolicy(params.policy);
    d->setRegistrySource(params.source);
    d->setSaveButton(params.saveButton);
    d->setDataChanged(params.dataChanged);
    d->setStateEnabled(params.stateEnabled);

    if (!params.presentation.widgets.empty())
    {
        QHBoxLayout *captions = createCaptions();
        layout->addLayout(captions);
    }

    for (const auto &widget : params.presentation.widgets)
    {
        std::string checkRes = widget.second->acceptCheck(*d);
        if (!checkRes.empty()){
            d->addLabel(checkRes);
        }
    }

    for (const auto &widget : params.presentation.widgets)
    {
        QWidget *policyWidget = nullptr;
        d->setCurrentElementName(widget.first);
        if (!widget.second->accept(*d)){
            d->popLabel();
        }

        if (policyWidget)
        {
            layout->addWidget(policyWidget);
        }
    }
    layout->addStretch();

    return layout;
}
} // namespace gui
