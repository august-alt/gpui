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

#include "inputmessagenotifier.h"
#include <iterator>
#include <algorithm>

namespace preferences {

bool WhitespaceDetector::detect(const QString& input)
{
    return input.trimmed() != input;
}
InputMessageNotifier::DetectElement::DetectElement(QWidget* parent, QLayout* layout, const QString& message, InputMessageNotifier::MessageLevel level)
{
    this->m_label = new QLabel(parent);
    layout->addWidget(this->m_label);

    this->m_label->hide();

    this->m_label->setText(message);

    switch(level)
    {
        case InputMessageNotifier::MessageLevel::Warning:
            this->m_label->setStyleSheet("background-color: rgb(249, 240, 107);\n"
                                         "color: rgb(0,0,0);\n"
                                         "border-radius: 10px;\n"
                                         "padding: 10px;");
        break;

        default:
        case InputMessageNotifier::MessageLevel::Error:
            this->m_label->setStyleSheet("background-color: rgb(246, 97, 81);\n"
                                         "color: rgb(255, 255, 255);\n"
                                         "border-radius: 10px;\n"
                                         "padding: 10px;");
        break;
    }

    this->m_level = level;
}
InputMessageNotifier::DetectElement::DetectElement(DetectElement&& element)
{
    this->m_detected = element.m_detected;
    this->m_label = element.m_label;
    this->m_level = element.m_level;
    element.m_label = nullptr;
}
InputMessageNotifier::DetectElement& InputMessageNotifier::DetectElement::operator=(DetectElement&& element)
{
    if (this->m_label)
    {
        delete this->m_label;
    }

    this->m_detected = element.m_detected;
    this->m_label = element.m_label;
    this->m_level = element.m_level;
    element.m_label = nullptr;

    return *this;
}

void InputMessageNotifier::DetectElement::detect()
{
    this->m_label->show();
    this->m_detected = true;
}
void InputMessageNotifier::DetectElement::undetect()
{
    this->m_label->hide();
    this->m_detected = false;
}

bool InputMessageNotifier::DetectElement::detected()
{
    return this->m_detected;
}

InputMessageNotifier::MessageLevel InputMessageNotifier::DetectElement::level()
{
    return this->m_level;
}

void InputMessageNotifier::DetectElement::setMessage(const QString& message)
{
    this->m_label->setText(message);
}

InputMessageNotifier::DetectElement::~DetectElement()
{
    if (this->m_label)
    {
        delete this->m_label;
    }
}

InputMessageNotifier::InputMessageNotifier(QWidget* widget)
: QWidget(widget)
{
    this->m_layout = new QVBoxLayout;
    this->setLayout(this->m_layout);
}

size_t InputMessageNotifier::addDetector(std::unique_ptr<InputDetector> detector)
{
    this->m_detectors[this->m_nextDetector] = std::move(detector);
    return this->m_nextDetector++;
}

void InputMessageNotifier::removeDetector(size_t id)
{
    this->m_detectors.erase(id);
}

void InputMessageNotifier::addInput(const QString& name)
{
    this->m_instances[name] = std::move(InputInstance());
}


void InputMessageNotifier::removeInput(const QString& name)
{
    this->m_instances.erase(name);
}

void InputMessageNotifier::updateInput(const QString& name, const QString& input)
{
    auto& instance = this->m_instances[name];

    for (auto& detect : instance)
    {
        bool detected = this->m_detectors[detect.first]->detect(input);

        if (detected && !detect.second.detected())
        {
            detect.second.detect();
            this->incCounter(detect.second.level());
        }
        else if(!detected && detect.second.detected())
        {
            detect.second.undetect();
            this->decCounter(detect.second.level());
        }
    }
}

void InputMessageNotifier::setMessage(const QString& name, size_t detector, const QString& message)
{
    auto entry = this->m_instances[name].find(detector);
    if (entry != this->m_instances[name].end())
    {
        entry->second.setMessage(message);
    }
}

void InputMessageNotifier::attachDetector(const QString& name, size_t detector, const QString& message, MessageLevel level)
{
    this->m_instances[name].insert(std::pair<size_t, DetectElement>(detector, DetectElement(this, this->m_layout, message, level)));
}

bool InputMessageNotifier::hasAnyError()
{
    return this->m_errorCount != 0;
}

bool InputMessageNotifier::hasAnyWarning()
{
    return this->m_warningCount != 0;
}

InputMessageNotifier::~InputMessageNotifier()
{

}

void InputMessageNotifier::incCounter(InputMessageNotifier::MessageLevel level)
{
    switch(level)
    {
    case InputMessageNotifier::MessageLevel::Warning:
        ++this->m_warningCount;
        break;
    case InputMessageNotifier::MessageLevel::Error:
        ++this->m_errorCount;
        break;
    }
}
void InputMessageNotifier::decCounter(MessageLevel level)
{
    switch(level)
    {
    case InputMessageNotifier::MessageLevel::Warning:
        --this->m_warningCount;
        break;
    case InputMessageNotifier::MessageLevel::Error:
        --this->m_errorCount;
        break;
    }
}

} // namespace preferences
