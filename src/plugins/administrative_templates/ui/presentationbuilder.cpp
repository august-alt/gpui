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

#include "../plugins/administrative_templates/admx/policybooleanelement.h"
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

QMap<std::string, QString> loadListFromRegistry(AbstractRegistrySource &m_source, const std::string &key, const std::string &prefix)
{
    QMap<std::string, QString> items;
    std::vector<std::string> valueNames = m_source.getValueNames(key);
    
    // finding and erase **delvals.
    for (auto it = valueNames.begin(); it != valueNames.end(); ++it) {
        if (QString::fromStdString(*it).compare("**delvals.", Qt::CaseInsensitive)) {
            it = valueNames.erase(it);

            // std::vecotor<T>::erase() returning iterator that pointing 
            // to the next element after erased
            --it; 
            break;
        }
    }
    
    for (auto &valueName : valueNames) {
        items[valueName] = 
                m_source.getValue(key, valueName).value<QString>();
    }
    return items;
}

void cleanUpListInRegistry(AbstractRegistrySource &m_source, const std::string &key)
{
    std::vector<std::string> valueNames = m_source.getValueNames(key);
    
    // just clean-up all values on this keypath
    for (auto &value : valueNames) {
        m_source.clearValue(key, value);
    }
}

void cleanUpListInRegistry(AbstractRegistrySource &m_source, const std::string &key, const std::string &prefix)
{
    // small optimization
    if (prefix.size() == 0)
    {
        cleanUpListInRegistry(m_source, key);
    }

    QString _prefix = QString::fromStdString(prefix);

    std::vector<std::string> valueNames = m_source.getValueNames(key);
    
    // clean-up all values that contain `prefix` prefix (case-insensitive)
    for (auto &value : valueNames) {
        if (value.size() > prefix.size() && 
            QString::fromUtf8(value.c_str(), prefix.size()).compare(_prefix, Qt::CaseInsensitive))
        {
            m_source.clearValue(key, value);
        }
    }
}

bool writeListIntoRegistry(AbstractRegistrySource &source, QMap<std::string, QString> valueList, const std::string &key, bool explicitValue, bool expandable, std::string &prefix)
{
    // https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2008-r2-and-2008/cc770327(v=ws.10)
    // true represents expandable string type (REG_EXPAND_SZ) and false represents string type (REG_SZ)
    auto type = expandable ? REG_EXPAND_SZ : REG_SZ;

    if (explicitValue)
    {
        // https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2008-r2-and-2008/cc731025(v=ws.10)
        // explicitValue cannot be used with the valuePrefix attribute.
        if (prefix.length() != 0)
        {
            // tmp warning msg.
            qWarning() << "Presentation builder::save: attempt to use explicitValue with the valuePrefix attribute";
        }

        for (auto begin = valueList.begin(), end = valueList.end(); begin != end; ++begin)
        {
            if (!begin.value().trimmed().isEmpty())
            {
                source.setValue(key, begin.key(), type, begin.value());
            }
        }
    }
    else 
    {
        // https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-gpreg/57226664-ce00-4487-994e-a6b3820f3e49
        // for non explicit values. Non-explicit value will be ignored.
        source.setValue(key, "**delvals.", REG_SZ, " ");

        size_t index = 1;
        for (auto begin = valueList.begin(), end = valueList.end(); begin != end; ++begin, ++index)
        {
            // https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2008-r2-and-2008/cc772195(v=ws.10)
            // valuePrefix represents the text string to be prepended to the incremented integer for registry subkey creation.
            if (!begin.value().trimmed().isEmpty())
            {
                source.setValue(key, begin.key(), type, QString::fromStdString(prefix) + QString::number(index));
            }
        }
    }

    return true;
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

private:
    static void processValue(AbstractRegistrySource *source, const ElementInfo &elementInfo, const BooleanValue &value)
    {
        const std::string &key =
                value.has_key ? value.key : elementInfo.key;

        qWarning() << "Presentation builder::save: " << key.c_str() << " "
                    << value.value_name.c_str();

        
        switch (value.type) {
        case BooleanValue::BOOLEAN_VALUE_TYPE_DELETED:
            source->clearValue(key, value.value_name);
            break;

        case BooleanValue::BOOLEAN_VALUE_TYPE_DECIMAL:
            source->setValue(key, value.value_name,
                                RegistryEntryType::REG_DWORD,
                                value.decimal);
            break;

        case BooleanValue::BOOLEAN_VALUE_TYPE_LONGDECIMAL:
            source->setValue(key, value.value_name,
                                RegistryEntryType::REG_QWORD,
                                value.long_decimal);
            break;

        case BooleanValue::BOOLEAN_VALUE_TYPE_STRING:
            source->setValue(key, value.value_name,
                                RegistryEntryType::REG_SZ,
                                QString::fromStdString(value.string));
            break;
        }
    }
    static void processValueList(AbstractRegistrySource *source, const ElementInfo &elementInfo, const PolicyBoolElement::BooleanList &booleanValueList)
    {
        for (const auto &el : booleanValueList)
        {
            processValue(source, elementInfo, el);
        }
    }

    static bool checkValue(AbstractRegistrySource *source, const ElementInfo &elementInfo, const BooleanValue &booleanValue)
    {
        const std::string &key =
                booleanValue.has_key ? booleanValue.key : elementInfo.key;
        
        if(source->isValuePresent(key, booleanValue.value_name))
        {
            QVariant value = source->getValue(key, booleanValue.value_name);

            switch (value.type()){
            case QVariant::UInt:
                if(booleanValue.type == BooleanValue::BOOLEAN_VALUE_TYPE_DECIMAL)
                {
                    return value.toUInt() == booleanValue.decimal;
                }
                return false;
            case QVariant::ULongLong:
                if(booleanValue.type == BooleanValue::BOOLEAN_VALUE_TYPE_LONGDECIMAL)
                {
                    return value.toULongLong() == booleanValue.long_decimal;
                }
                return false;
            break;
            case QVariant::String:
                if(booleanValue.type == BooleanValue::BOOLEAN_VALUE_TYPE_STRING)
                {
                    return value.toString().toStdString() == booleanValue.string;
                }
                return false;
            break;
            default:
            return false;
            }
            return false;
        }
        if(booleanValue.type == BooleanValue::BOOLEAN_VALUE_TYPE_DELETED)
        {
            return true;
        }
        return false;
    }
    static bool checkValueList(AbstractRegistrySource *source, const ElementInfo &elementInfo, const PolicyBoolElement::BooleanList &booleanValueList)
    {
        for (const auto &el : booleanValueList)
        {
            if(!checkValue(source, elementInfo, el))
            {
                return false;
            }
        }
        return true;
    }

    static bool checkStatus(AbstractRegistrySource *source, const ElementInfo &elementInfo)
    {
        const auto booleanElement =
                dynamic_cast<PolicyBoolElement *>(elementInfo.element);
        if (booleanElement == nullptr) {
            qWarning() << "Presentation builder::builder: the element attached to the checkbox is not a BooleanElement";
            return false;
        }

        if (!source->isValuePresent(elementInfo.key, elementInfo.value)) {
            return false;
        }
        if (booleanElement->hasTrueValue || booleanElement->hasFalseValue)
        {
            return checkValue(source, elementInfo, booleanElement->trueValue);
        }
        if(booleanElement->hasTrueList || booleanElement->hasFalseList)
        {
            return checkValueList(source, elementInfo, booleanElement->trueList);
        }
        return source->getValue(elementInfo.key, elementInfo.value).value<bool>();
    }

public:
    virtual void visit(CheckBox &widget) const override
    {
        QCheckBox *checkBox = new QCheckBox();

        checkBox->setChecked(widget.defaultChecked);
        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(checkBox, QString::fromStdString(widget.label));

        if (m_policy && m_source)
        {
            const ElementInfo elementInfo = findElementInfo();
            const auto booleanElement =
                    dynamic_cast<PolicyBoolElement *>(elementInfo.element);

            if (booleanElement == nullptr) {
                qWarning() << "Presentation builder::builder: the element attached to the checkbox is not a BooleanElement";
                return;
            }

            checkBox->setChecked(checkStatus(m_source, elementInfo));
            checkBox->connect(checkBox, &QCheckBox::toggled, []() { *m_dataChanged = true; });

            // TODO: Implement correct type on save.
            m_saveButton->connect(
                    m_saveButton, &QPushButton::clicked, [elementInfo, checkBox, booleanElement, this]() {
                        int checked = checkBox->checkState() == Qt::Checked ? 1 : 0;

                        if (!(*m_stateEnabled)) {
                            return;
                        }
                        if (!booleanElement) {
                            qWarning() << "Presentation builder::builder: the element attached to the checkbox is not a BooleanElement";
                            return;
                        }
                        if(booleanElement->hasFalseList != booleanElement->hasTrueList)
                        {
                            qWarning() << "Presentation builder::save: one of trueList/falseList is't present";
                        }
                        if (booleanElement->hasTrueValue != booleanElement->hasFalseValue)
                        {
                            qWarning() << "Presentation builder::save: one of trueValue/falseValue is't present";
                        }

                        // If has one of list.
                        if (booleanElement->hasFalseList || booleanElement->hasTrueList) {
                            const auto &setList = checked
                                    ? booleanElement->trueList
                                    : booleanElement->falseList;

                            processValueList(m_source, elementInfo, setList);
                        }

                        // If we has one of trueValue/falseValue, then we don't calling setValue for elementiInfo.key elementInfo.value
                        if (booleanElement->hasTrueValue || booleanElement->hasFalseValue)
                        {
                            processValue(m_source, elementInfo, checked ? booleanElement->trueValue : booleanElement->falseValue);
                            return;
                        }

                        qWarning() << "Presentation builder::save: " << elementInfo.key.c_str()
                                << " " << elementInfo.value.c_str();
                        m_source->setValue(elementInfo.key, elementInfo.value,
                                        RegistryEntryType::REG_DWORD, checked);
            });
        }

        addToLayout(container);
    }

    virtual void visit(ComboBox &widget) const override
    {
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

        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(comboBox, QString::fromStdString(widget.label));

        addToLayout(container);
    }

    virtual void visit(DecimalTextBox &widget) const override
    {
        QWidget *textBox = createAnyDecimalTextBox(widget.spin, widget.defaultValue, widget.spinStep);

        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(textBox, QString::fromStdString(widget.label));

        addToLayout(container);
    }

    virtual void visit(DropdownList &widget) const override
    {
        QComboBox *comboBox = new QComboBox();
        comboBox->setCurrentIndex(widget.defaultItem);

        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(comboBox, QString::fromStdString(widget.label));

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
    }

    virtual void visit(ListBox &widget) const override
    {
        QPushButton *button = new QPushButton(QObject::tr("Edit"));

        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(button, QString::fromStdString(widget.label));

        PolicyListElement* listElement = nullptr;

        ElementInfo elementInfo;

        if (m_policy && m_source)
        {
             elementInfo = findElementInfo();

            if ((listElement = dynamic_cast<PolicyListElement *>(elementInfo.element)))
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

            // https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2008-r2-and-2008/cc731025(v=ws.10)
            // explicitValue cannot be used with the valuePrefix attribute.
            if (listElement->valuePrefix.size() > 0 && listElement->explicitValue)
            {
                qWarning() << "Unable to get valid policy listElement (explicitValue cannot be used with the valuePrefix attribute).";
                return;
            }

            RegistryEntryType registryEntryType = listElement->expandable ? RegistryEntryType::REG_EXPAND_SZ
                                                                          : RegistryEntryType::REG_SZ;

            if (listElement->explicitValue)
            {
                // two collumn ListBox
                listBox->setItems(loadListFromRegistry(*m_source, listElement->key, listElement->valuePrefix));
            }
            else 
            {
                // one collumn ListBox
                listBox->setItems(loadListFromRegistry(*m_source, listElement->key, listElement->valuePrefix).values());
            }

            listBox->connect(listBox,
                                &gpui::ListBoxDialog::itemsEditingFinished,
                                [=](QMap<std::string, QString> currentItems) {
                if (!(*m_stateEnabled))
                {
                    return;
                }
                qWarning() << "Items debug: " << currentItems.values();

                cleanUpListInRegistry(*m_source, listElement->key, listElement->valuePrefix);

                writeListIntoRegistry(*m_source, 
                                      currentItems, 
                                      listElement->key, 
                                      listElement->explicitValue, 
                                      listElement->expandable, 
                                      listElement->valuePrefix);

                *m_dataChanged = true;
            });

            listBox->show();
        };

        QObject::connect(button, &QPushButton::clicked, onClicked);

        addToLayout(container);
    }

    virtual void visit(LongDecimalTextBox &widget) const override
    {
        QWidget *textBox = createAnyDecimalTextBox(widget.spin, widget.defaultValue, widget.spinStep);

        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(textBox, QString::fromStdString(widget.label));

        addToLayout(container);
    }

    virtual void visit(MultiTextBox &widget) const override
    {
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
                qWarning() << "Presentation builder::save: " << elementInfo.key.c_str() << " "
                           << elementInfo.value.c_str();
                QStringList data(textEdit->toPlainText());
                m_source->setValue(elementInfo.key, elementInfo.value, RegistryEntryType::REG_MULTI_SZ, data);
            });
        }

        addToLayout(textEdit);
    }

    virtual void visit(Text &widget) const override
    {
        QLabel *label = new QLabel();
        label->setText(QString::fromStdString(widget.content));
        label->setWordWrap(true);
        label->setAlignment(Qt::AlignHCenter);
        addToLayout(label);
    }

    virtual void visit(TextBox &widget) const override
    {
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
                qWarning() << "Presentation builder::save: " << elementInfo.key.c_str() << " "
                           << elementInfo.value.c_str();
                QString data(lineEdit->text());
                m_source->setValue(elementInfo.key,
                                   elementInfo.value,
                                   RegistryEntryType::REG_SZ,
                                   QVariant::fromValue(data));
            });
        }

        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(lineEdit, QString::fromStdString(widget.label));

        addToLayout(container);
    }

    void setLayout(QLayout *layout) { m_layout = layout; }

    void setPolicy(const Policy &policy) { m_policy = &policy; }

    void setRegistrySource(AbstractRegistrySource &source) { m_source = &source; }

    void setCurrentElementName(std::string elementName) { m_elementName = elementName; }

    void setSaveButton(QPushButton &saveButton) { m_saveButton = &saveButton; }

    void setDataChanged(bool &dataChanged) { m_dataChanged = &dataChanged; }

    void setStateEnabled(bool &stateEnabled) { m_stateEnabled = &stateEnabled; }

private:
    QLayout *m_layout                = nullptr;
    const Policy *m_policy           = nullptr;
    AbstractRegistrySource *m_source = nullptr;
    QPushButton *m_saveButton        = nullptr;
    std::string m_elementName        = "";

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

    for (const auto &widget : params.presentation.widgetsVector)
    {
        QWidget *policyWidget = nullptr;
        d->setCurrentElementName(widget.second);
        widget.first->accept(*d);

        if (policyWidget)
        {
            layout->addWidget(policyWidget);
        }
    }
    layout->addStretch();

    return layout;
}
} // namespace gui
