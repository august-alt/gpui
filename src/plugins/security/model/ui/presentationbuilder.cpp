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

#include "presentationbuilder.h"

#include <QVBoxLayout>

#include "../presentation/checkbox.h"
#include "../presentation/combobox.h"
#include "../presentation/decimaltextbox.h"
#include "../presentation/dropdownlist.h"
#include "../presentation/groupbox.h"
#include "../presentation/ldapsearchdialog.h"
#include "../presentation/listbox.h"
#include "../presentation/longdecimaltextbox.h"
#include "../presentation/multitextbox.h"
#include "../presentation/text.h"
#include "../presentation/textbox.h"

#include "../presentation/securitypresentationresources.h"

#include <type_traits>
#include <variant>

#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QIntValidator>
#include <QComboBox>
#include <QGroupBox>
#include <QRadioButton>

#include <gui/altspinbox.h>

template<class>
inline constexpr bool always_false_v = false;

namespace security
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

class PresentationBuilderPrivate
{
public:

    void visit(DecimalTextBox &widget) const
    {
        QWidget *textBox = createAnyDecimalTextBox(widget.spinBox, widget.defaultValue, widget.spinStep);

        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(textBox, QString::fromStdString(widget.postfix));

        addToLayout(container);
    }

    void visit(TextBox &widget) const
    {
        QLineEdit *lineEdit = new QLineEdit();
        lineEdit->setText(QString::fromStdString(widget.defaultValue));

        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(lineEdit, QString::fromStdString(widget.label));

        addToLayout(container);
    }

    void visit(CheckBox &widget) const
    {
        QCheckBox *checkBox = new QCheckBox();

        checkBox->setChecked(widget.defaultChecked);
        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(checkBox, QString::fromStdString(widget.postfix));

        addToLayout(container);
    }

    void visit(ComboBox &widget) const
    {
        QComboBox *comboBox = new QComboBox();
        comboBox->setCurrentText(QString::fromStdString(widget.defaultValue));
        for (const auto &item : widget.suggestion)
        {
            comboBox->addItem(QString::fromStdString(item));
        }

        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(comboBox, QString::fromStdString(widget.label));

        addToLayout(container);
    }

    void visit(DropdownList &widget) const
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

        addToLayout(container);
    }

    void visit(LongDecimalTextBox &widget) const
    {
        QWidget *textBox = createAnyDecimalTextBox(widget.spinBox, widget.defaultValue, widget.spinStep);

        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(textBox, QString::fromStdString(widget.postfix));

        addToLayout(container);
    }

    void visit(MultiTextBox &widget) const
    {
        Q_UNUSED(widget);
        QTextEdit *textEdit = new QTextEdit();

        // TODO: Implement.

        addToLayout(textEdit);
    }

    void visit(GroupBox &widget) const
    {
        Q_UNUSED(widget);
        QGroupBox *groupBox = new QGroupBox();

        // TODO: Implement.

        addToLayout(groupBox);
    }

    void visit(ListBox &widget) const
    {
        // TODO: Implement.

        Q_UNUSED(widget);
    }

    void visit(LdapSearchDialog &widget) const
    {
        // TODO: Implement.

        Q_UNUSED(widget);
    }

    void visit(RadioButton &widget) const
    {
        QRadioButton *radioButton = new QRadioButton();

        radioButton->setChecked(widget.defaultChecked);
        QLayoutItem *container = createAndAttachLabel<QHBoxLayout>(radioButton, "");

        addToLayout(container);
    }

    void visit(Comment &widget) const
    {
        QLabel *label = new QLabel();
        label->setText(QString::fromStdString(widget.text));
        label->setWordWrap(true);
        label->setAlignment(Qt::AlignHCenter);
        addToLayout(label);
    }

    void visit(Text &widget) const
    {
        QLabel *label = new QLabel();
        label->setText(QString::fromStdString(widget.content));
        label->setWordWrap(true);
        label->setAlignment(Qt::AlignHCenter);
        addToLayout(label);
    }

    void setLayout(QLayout *layout) { m_layout = layout; }

    void setSecurity(const SecurityDefinition &security) { m_security = &security; }

    void setCurrentElementName(std::string elementName) { m_elementName = elementName; }

private:
    QLayout *m_layout                    = nullptr;
    const SecurityDefinition *m_security = nullptr;
    std::string m_elementName            = "";

private:
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

private:
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

            return spinBox;
        }

        QLineEdit *edit = new QLineEdit();
        edit->setText(QString::number(value));
        edit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max()));

        return edit;
    }
};

PresentationBuilderPrivate* PresentationBuilder::d = new PresentationBuilderPrivate();

QVBoxLayout *PresentationBuilder::build(const PresentationBuilderParams &params)
{
    QVBoxLayout *layout = new QVBoxLayout();
    d->setLayout(layout);
    d->setSecurity(params.security);

    for (const auto &widget : params.presentation.widgets)
    {
        d->setCurrentElementName(widget.first);

        std::visit([](auto&& arg) noexcept
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::shared_ptr<DecimalTextBox> >)
            {
                d->visit(*arg);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<TextBox> >)
            {
                d->visit(*arg);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<CheckBox> >)
            {
                d->visit(*arg);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<ComboBox> >)
            {
                d->visit(*arg);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<DropdownList> >)
            {
                d->visit(*arg);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<LongDecimalTextBox> >)
            {
                d->visit(*arg);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<MultiTextBox> >)
            {
                d->visit(*arg);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<GroupBox> >)
            {
                d->visit(*arg);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<ListBox> >)
            {
                d->visit(*arg);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Comment> >)
            {
                d->visit(*arg);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<Text> >)
            {
                d->visit(*arg);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<LdapSearchDialog> >)
            {
                d->visit(*arg);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<RadioButton> >)
            {
                d->visit(*arg);
            }
            else
            {
                static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }
        }, widget.second);
    }
    layout->addStretch();

    return layout;
}

}
