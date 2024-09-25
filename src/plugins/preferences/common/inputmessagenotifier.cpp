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

InputMessageNotifier::InputMessageNotifier(QWidget *parent) : QWidget(parent)
{
    this->m_layout = new QVBoxLayout;
    this->setLayout(this->m_layout);
/*
    for (size_t i = 0; i < static_cast<size_t>(MessageNotifierType::MESSAGE_NOTIFIER_LENGTH); ++i) {
        this->m_messages[i] = new QLabel(this);

        this->m_messages[i]->setText(this->m_getMessages[i]());
        this->m_messages[i]->hide();
        this->m_messages[i]->setStyleSheet("background-color: rgb(249, 240, 107);\n"
                                           "border-radius: 10px;\n"
                                           "padding: 10px;");

        this->m_layout->addWidget(this->m_messages[i]);
    }*/
}

size_t InputMessageNotifier::addDetector(std::unique_ptr<IInputMessageDetector> detector)
{
    if (!detector)
    {
        throw std::runtime_error("LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                + ", Encountered with null pointer raither of detector.")
    }
    this->m_detectors[this->m_next_detector++] = std::move(detector);
}

void InputMessageNotifier::addInstance(const QString& name)
{
    this->m_instances[name] = MessageInstance();
}
void InputMessageNotifier::eraseInstance(const QString& name)
{
    this->m_instances.erase(name);
    updateState()
}
void InputMessageNotifier::setInstanceDetection(const QString& name, size_t type, bool detect = true)
{
    if (detect)
    {
        this->m_instances[name].mask.insert(type);
    }
    else
    {
        this->m_instances[name].mask.erase(type);
    }
}
void InputMessageNotifier::updateInstance(const QString& name, const QString &content)
{
    auto& instance = this->m_instances[name];

    instance.content = content;

    for(auto& detector : this->m_detectors)
    {
        if (instance.mask.find(detector.first) != instance.mask.end() && detector.second->detect(content))
        {
            instance.detected.insert(detector.first);
        }
        else 
        {
            instance.detected.erase(detector.first);
        }
    }

    updateState();
}

void InputMessageNotifier::updateState()
{
    std::set<size_t> types;

    for (auto& instance : this->m_instances)
    {
        types.insert(instance.second.detected.begin(), instance.second.detected.end());
    }

    for (auto& detector : this->m_detectors)
    {
        if (types.find(detector.first) != types.end())
        {
            this->m_detectors[detector.first]->raw()->show();
        }
        else 
        {
            this->m_detectors[detector.first]->raw()->hide();
        }
    }
}

void InputMessageNotifier::retranslateUi()
{

}

} // namespace preferences
