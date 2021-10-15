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

#include "../model/presentation/presentationwidget.h"
#include "../model/presentation/presentationwidgetvisitor.h"
#include "../model/presentation/presentation.h"

#include "../model/presentation/checkbox.h"
#include "../model/presentation/combobox.h"
#include "../model/presentation/decimaltextbox.h"
#include "../model/presentation/dropdownlist.h"
#include "../model/presentation/listbox.h"
#include "../model/presentation/longdecimaltextbox.h"
#include "../model/presentation/multitextbox.h"
#include "../model/presentation/text.h"
#include "../model/presentation/textbox.h"

#include "../model/admx/policy.h"
#include "../model/admx/policyelement.h"

#include "../model/commands/command.h"
#include "../model/commands/commandgroup.h"

#include "../model/registry/abstractregistrysource.h"

#include <QVBoxLayout>

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QPlainTextEdit>
#include <QTableWidget>
#include <QHeaderView>

#include <QDebug>

#include <iostream>

using namespace model::presentation;
using namespace model::admx;
using namespace model::registry;
using namespace model::command;

namespace gui
{
    template <typename TLayoutItem>
    QLayoutItem* createAndAttachLabel(QWidget* buddy, const QString& text)
    {
        QLabel* label = new QLabel(text.trimmed());
        label->setBuddy(buddy);
        label->setWordWrap(true);

        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(1);

        label->setSizePolicy(sizePolicy);
        buddy->setSizePolicy(sizePolicy);

        TLayoutItem* container = new TLayoutItem();
        container->addWidget(label);
        container->addWidget(buddy);

        return container;
    }

    QHBoxLayout* createCaptions()
    {
        QHBoxLayout* horizontalLayout = new QHBoxLayout();
        QLabel* descriptionLabel = new QLabel(QObject::tr("Description:"));
        QLabel* optionLabel = new QLabel(QObject::tr("Options:"));
        horizontalLayout->addWidget(descriptionLabel);
        horizontalLayout->addWidget(optionLabel);
        return horizontalLayout;
    }

    class PresentationBuilderPrivate : public PresentationWidgetVisitor {
    public:
        virtual void visit(CheckBox &widget) const override
        {
            QCheckBox* checkBox = new QCheckBox();

            checkBox->setChecked(widget.defaultChecked);
            QLayoutItem* container = createAndAttachLabel<QHBoxLayout>(checkBox, QString::fromStdString(widget.label));

            if (m_policy && m_source)
            {
                std::pair<std::string, std::string> keyValuePair = findKeyAndValueName();

                if (m_source->isValuePresent(keyValuePair.first, keyValuePair.second))
                {
                    checkBox->setChecked(m_source->getValue(keyValuePair.first, keyValuePair.second).value<bool>());
                }

                checkBox->connect(checkBox, &QCheckBox::toggled, [=](bool checked) {
                    createCommand([=](){
                        m_source->setValue(keyValuePair.first, keyValuePair.second, RegistryEntryType::REG_DWORD, checked);
                    });
                });
            }

            addToLayout(container);
        }

        virtual void visit(ComboBox &widget) const override
        {
            QComboBox* comboBox = new QComboBox();
            comboBox->setCurrentText(QString::fromStdString(widget.defaultValue));
            for (const auto& item: widget.suggestions)
            {
                comboBox->addItem(QString::fromStdString(item));
            }

            if (m_policy && m_source)
            {
                std::pair<std::string, std::string> keyValuePair = findKeyAndValueName();

                if (m_source->isValuePresent(keyValuePair.first, keyValuePair.second))
                {
                    comboBox->setCurrentIndex(m_source->getValue(keyValuePair.first, keyValuePair.second).value<uint32_t>());
                }

                comboBox->connect(comboBox,  QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
                    createCommand([=](){
                        m_source->setValue(keyValuePair.first, keyValuePair.second, RegistryEntryType::REG_DWORD, index);
                    });
                });
            }

            QLayoutItem* container = createAndAttachLabel<QHBoxLayout>(comboBox, QString::fromStdString(widget.label));

            addToLayout(container);
        }

        virtual void visit(DecimalTextBox &widget) const override
        {
            QWidget* textBox = createAnyDecimalTextBox(widget.spin, widget.defaultValue, widget.spinStep);

            QLayoutItem* container = createAndAttachLabel<QHBoxLayout>(textBox, QString::fromStdString(widget.label));

            addToLayout(container);
        }

        virtual void visit(DropdownList &widget) const override
        {
            QComboBox* comboBox = new QComboBox();
            comboBox->setCurrentIndex(widget.defaultItem);

            QLayoutItem* container = createAndAttachLabel<QHBoxLayout>(comboBox, QString::fromStdString(widget.label));

            if (widget.values.size() > 0) {
                for (auto& value : widget.values)
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
                std::pair<std::string, std::string> keyValuePair = findKeyAndValueName();

                if (m_source->isValuePresent(keyValuePair.first, keyValuePair.second))
                {
                    comboBox->setCurrentIndex(m_source->getValue(keyValuePair.first, keyValuePair.second).value<uint32_t>());
                }

                comboBox->connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
                    createCommand([=](){
                        m_source->setValue(keyValuePair.first, keyValuePair.second, RegistryEntryType::REG_DWORD, index);
                    });
                });
            }

            addToLayout(container);
        }

        virtual void visit(ListBox &widget) const override
        {
            QTableWidget* listBox = new QTableWidget();
            listBox->setColumnCount(2);
            listBox->setRowCount(1);
            QStringList header { "#", "Name" };
            listBox->setHorizontalHeaderLabels(header);
            listBox->setSelectionBehavior(QAbstractItemView::SelectRows);
            listBox->setSelectionMode(QAbstractItemView::SingleSelection);
            listBox->setShowGrid(true);
            listBox->horizontalHeader()->setStretchLastSection(true);
            listBox->hideColumn(0);

            QLayoutItem* container = createAndAttachLabel<QHBoxLayout>(listBox, QString::fromStdString(widget.label));

            if (m_policy && m_source)
            {
                std::pair<std::string, std::string> keyValuePair = findKeyAndValueName();

                if (m_source->isValuePresent(keyValuePair.first, keyValuePair.second))
                {
                    QStringList items = m_source->getValue(keyValuePair.first, keyValuePair.second).value<QStringList>();
                    qWarning() << "Items debug: " << items;
                    int index = 0;
                    listBox->setRowCount(items.size());
                    for (const auto& itemString : items)
                    {
                        listBox->setItem(index, 1, new QTableWidgetItem(itemString));
                        index++;
                    }
                }

                listBox->connect(listBox, &QTableWidget::itemChanged, [=]() {
                    listBox->insertRow(listBox->rowCount());

                    createCommand([=](){
                        QStringList items;
                        for(int i = 0; i < listBox->rowCount(); ++i)
                        {
                            QTableWidgetItem* item = listBox->item(i, 1);
                            if (item && item->data(Qt::DisplayRole).isValid())
                            {
                                items.push_back(item->text());
                            }
                        }
                        m_source->setValue(keyValuePair.first, keyValuePair.second, RegistryEntryType::REG_MULTI_SZ, items);
                    });
                });
            }

            addToLayout(container);
        }

        virtual void visit(LongDecimalTextBox &widget) const override
        {
            QWidget* textBox = createAnyDecimalTextBox(widget.spin, widget.defaultValue, widget.spinStep);

            QLayoutItem* container = createAndAttachLabel<QHBoxLayout>(textBox, QString::fromStdString(widget.label));

            addToLayout(container);
        }

        virtual void visit(MultiTextBox &widget) const override
        {
            QTextEdit* textEdit = new QTextEdit();
            textEdit->setMaximumHeight(widget.defaultHeight * textEdit->fontMetrics().height());

            if (m_policy && m_source)
            {
                std::pair<std::string, std::string> keyValuePair = findKeyAndValueName();

                if (m_source->isValuePresent(keyValuePair.first, keyValuePair.second))
                {
                    auto value = m_source->getValue(keyValuePair.first, keyValuePair.second).value<QString>();
                    textEdit->setPlainText(value);
                }

                textEdit->connect(textEdit, &QTextEdit::textChanged, [=](){
                    createCommand([=](){
                        QStringList data(textEdit->toPlainText());
                        m_source->setValue(keyValuePair.first, keyValuePair.second, RegistryEntryType::REG_MULTI_SZ, data);
                    });
                });
            }

            addToLayout(textEdit);
        }

        virtual void visit(Text &widget) const override
        {
            QLabel* label = new QLabel();
            label->setText(QString::fromStdString(widget.content));
            label->setWordWrap(true);
            label->setAlignment(Qt::AlignHCenter);
            addToLayout(label);
        }

        virtual void visit(TextBox &widget) const override
        {
            QLineEdit* lineEdit = new QLineEdit();
            lineEdit->setText(QString::fromStdString(widget.defaultValue));

            if (m_policy && m_source)
            {
                std::pair<std::string, std::string> keyValuePair = findKeyAndValueName();

                if (m_source->isValuePresent(keyValuePair.first, keyValuePair.second))
                {
                    auto value = m_source->getValue(keyValuePair.first, keyValuePair.second).value<QString>();
                    lineEdit->setText(value);
                }

                lineEdit->connect(lineEdit, &QLineEdit::textChanged, [=](const QString& text){
                    createCommand([=](){
                        m_source->setValue(keyValuePair.first, keyValuePair.second, RegistryEntryType::REG_SZ, QVariant::fromValue(text));
                    });
                });
            }

            QLayoutItem* container = createAndAttachLabel<QHBoxLayout>(lineEdit, QString::fromStdString(widget.label));

            addToLayout(container);
        }

        void setLayout(QLayout* layout) {
            m_layout = layout;
        }

        void setPolicy(const Policy& policy)
        {
            m_policy = &policy;
        }

        void setRegistrySource(AbstractRegistrySource& source)
        {
            m_source = &source;
        }

        void setCurrentElementName(std::string elementName)
        {
            m_elementName = elementName;
        }

        void setCommandGroup(CommandGroup& commandGroup)
        {
            m_commandGroup = &commandGroup;
        }

    private:
        QLayout* m_layout = nullptr;
        const Policy* m_policy = nullptr;
        AbstractRegistrySource* m_source = nullptr;
        CommandGroup* m_commandGroup = nullptr;
        std::string m_elementName = "";

        void addToLayout(QWidget* widget) const {
            if (m_layout) {
                m_layout->addWidget(widget);
            }
        }

        void addToLayout(QLayoutItem* container) const
        {
            if (container)
            {
                m_layout->addItem(container);
            }
        }

        template<typename Number>
        QWidget* createAnyDecimalTextBox(bool spin, Number value, Number step) const {
            if (spin)
            {
                QSpinBox* spinBox = new QSpinBox();
                spinBox->setMinimum(0);
                spinBox->setMaximum(9999);
                spinBox->setSingleStep(step);
                spinBox->setValue(value);

                if (m_policy && m_source)
                {
                    std::pair<std::string, std::string> keyValuePair = findKeyAndValueName();

                    if (m_source->isValuePresent(keyValuePair.first, keyValuePair.second))
                    {
                        spinBox->setValue(m_source->getValue(keyValuePair.first, keyValuePair.second).value<Number>());
                    }

                    spinBox->connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
                        createCommand([=](){
                            m_source->setValue(keyValuePair.first, keyValuePair.second, RegistryEntryType::REG_DWORD, value);
                        });
                    });
                }

                return spinBox;
            }

            QLineEdit* edit = new QLineEdit();
            edit->setText(QString::number(value));
            edit->setValidator(new QIntValidator(0, 9999));

            if (m_policy && m_source)
            {
                std::pair<std::string, std::string> keyValuePair = findKeyAndValueName();

                if (m_source->isValuePresent(keyValuePair.first, keyValuePair.second))
                {
                    edit->setText(QString(m_source->getValue(keyValuePair.first, keyValuePair.second).value<Number>()));
                }

                edit->connect(edit, &QLineEdit::textChanged, [=](const QString & value) {
                    createCommand([=](){
                        m_source->setValue(keyValuePair.first, keyValuePair.second, RegistryEntryType::REG_DWORD, value.toUInt());
                    });
                });
            }

            return edit;
        }

        std::pair<std::string, std::string> findKeyAndValueName() const
        {
            if (m_policy && m_source)
            {
                for (const auto& element : m_policy->elements)
                {
                    if (element->id.compare(m_elementName) == 0)
                    {
                        if (element->key.size() > 0)
                        {
                            return std::make_pair(element->key, element->valueName);
                        } else {
                            return std::make_pair(m_policy->key, element->valueName);
                        }
                    }
                }
            }

            qWarning() << "Key and value not found!" << m_elementName.c_str();

            return std::make_pair("", "");
        }

        void createCommand(std::function<void()> function) const
        {
            auto command = std::make_unique<LambdaCommand>(function);
            m_commandGroup->addSubCommand(std::move(command));
        }
    };

    PresentationBuilderPrivate* PresentationBuilder::d = new PresentationBuilderPrivate();

    QVBoxLayout* PresentationBuilder::build(const Presentation& presentation,
                                            const model::admx::Policy &policy,
                                            model::registry::AbstractRegistrySource &source,
                                            model::command::CommandGroup &commandGroup)
    {
        QVBoxLayout* layout = new QVBoxLayout();
        d->setLayout(layout);
        d->setPolicy(policy);
        d->setRegistrySource(source);
        d->setCommandGroup(commandGroup);

        QHBoxLayout* captions = createCaptions();
        layout->addLayout(captions);

        for (const auto& widget : presentation.widgets) {
            QWidget* policyWidget = nullptr;
            d->setCurrentElementName(widget.first);
            widget.second->accept(*d);

            if (policyWidget) {
                layout->addWidget(policyWidget);
            }
        }
        layout->addStretch();

        return layout;
    }
}
