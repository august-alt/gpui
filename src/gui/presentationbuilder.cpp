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

#include <QVBoxLayout>

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QPlainTextEdit>

#include <iostream>

using namespace model::presentation;

namespace gui
{
    class PresentationBuilderPrivate : public PresentationWidgetVisitor {
    public:
        virtual void visitCheckBox(const CheckBox &widget) const override
        {
            QLabel* label = new QLabel(QString::fromStdString(widget.label));
            QCheckBox* checkBox = new QCheckBox();

            checkBox->setChecked(widget.defaultChecked);
            label->setBuddy(checkBox);
            label->setWordWrap(true);

            QHBoxLayout* container = new QHBoxLayout();
            container->addWidget(checkBox);
            container->addSpacing(8);
            container->addWidget(label);
            container->addStretch();

            addToLayout(container);
        }

        virtual void visitComboBox(const ComboBox &widget) const override
        {
            QComboBox* comboBox = new QComboBox();
            comboBox->setCurrentText(QString::fromStdString(widget.defaultValue));
            for (const auto& item: widget.suggestions)
            {
                comboBox->addItem(QString::fromStdString(item));
            }

            QLabel* label = new QLabel(QString::fromStdString(widget.label));
            label->setBuddy(comboBox);
            label->setWordWrap(true);

            QVBoxLayout* container = new QVBoxLayout();
            container->addWidget(label);
            container->addWidget(comboBox);

            addToLayout(container);
        }

        virtual void visitDecimalTextBox(const DecimalTextBox &widget) const override
        {
            createAnyDecimalTextBox(widget.spin, widget.defaultValue, widget.spinStep);
        }

        virtual void visitDropdownList(const DropdownList &widget) const override
        {
            QComboBox* comboBox = new QComboBox();
            comboBox->setCurrentIndex(widget.defaultItem);

            QLabel* label = new QLabel(QString::fromStdString(widget.label));
            label->setBuddy(comboBox);
            label->setWordWrap(true);

            QVBoxLayout* container = new QVBoxLayout();
            container->addWidget(label);
            container->addWidget(comboBox);

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

            addToLayout(container);
        }

        virtual void visitListBox(const ListBox &widget) const override
        {
            QListWidget* listWidget = new QListWidget();

            QLabel* label = new QLabel(QString::fromStdString(widget.label));
            label->setBuddy(listWidget);
            label->setWordWrap(true);

            QVBoxLayout* container = new QVBoxLayout();
            container->addWidget(label);
            container->addWidget(listWidget);

            addToLayout(container);
        }

        virtual void visitLongDecimalTextBox(const LongDecimalTextBox &widget) const override
        {
            createAnyDecimalTextBox(widget.spin, widget.defaultValue, widget.spinStep);
        }

        virtual void visitMultiTextBox(const MultiTextBox &widget) const override
        {
            QTextEdit* textEdit = new QTextEdit();
            textEdit->setMaximumHeight(widget.defaultHeight * textEdit->fontMetrics().height());
            addToLayout(textEdit);
        }

        virtual void visitText(const Text &widget) const override
        {
            QLabel* label = new QLabel();
            label->setText(QString::fromStdString(widget.content));
            label->setWordWrap(true);
            addToLayout(label);
        }

        virtual void visitTextBox(const TextBox &widget) const override
        {
            QLineEdit* lineEdit = new QLineEdit();
            lineEdit->setText(QString::fromStdString(widget.defaultValue));

            QLabel* label = new QLabel(QString::fromStdString(widget.label));
            label->setBuddy(lineEdit);
            label->setWordWrap(true);

            QVBoxLayout* container = new QVBoxLayout();
            container->addWidget(label);
            container->addWidget(lineEdit);

            addToLayout(container);
        }

        void setLayout(QLayout* layout) {
            m_layout = layout;
        }

    private:
        QLayout* m_layout = nullptr;

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
        void createAnyDecimalTextBox(bool spin, Number value, Number step) const {
            if (spin)
            {
                QSpinBox* spinBox = new QSpinBox();
                // TODO: Replace maximum and minimum values with parameters specified in ADMX/ADML documents.
                spinBox->setMinimum(0);
                spinBox->setMaximum(9999);
                spinBox->setSingleStep(step);
                spinBox->setValue(value);
                addToLayout(spinBox);
                return;
            }

            QLineEdit* edit = new QLineEdit();
            edit->setText(QString::number(value));
            // TODO: Create int validator for decimal text box.
            edit->setValidator(new QIntValidator(0, 9999));
            addToLayout(edit);
        }
    };

    PresentationBuilderPrivate* PresentationBuilder::d = new PresentationBuilderPrivate();

    QWidget* buildWidget(const CheckBox* widget)
    {
        QCheckBox* checkBox = new QCheckBox();
        checkBox->setChecked(widget->defaultChecked);
        return checkBox;
    }

    QVBoxLayout* PresentationBuilder::build(const Presentation& presentation)
    {
        QVBoxLayout* layout = new QVBoxLayout();
        d->setLayout(layout);

        for (const auto& widget : presentation.widgetsVector) {
            QWidget* policyWidget = nullptr;
            widget->accept(*d);

            if (policyWidget) {
                layout->addWidget(policyWidget);
            }
        }
        layout->addStretch();

        return layout;
    }
}
