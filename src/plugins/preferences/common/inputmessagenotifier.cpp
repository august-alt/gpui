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

namespace preferences {

QString InputMessageNotifier::invisibleSpaceMessage()
{
    return tr("The input field has a space at the beginning or at the end");
}
QString InputMessageNotifier::dotsMessage()
{
    return tr("There is a folder '.'/'..' in the path input field");
}
QString InputMessageNotifier::folderPathMessage()
{
    return tr("The path input field contains the path to the folder");
}
QString InputMessageNotifier::windowsCompatibilityMessage()
{
    return tr("The path input field contains a path that is not compatible with windows");
}
QString InputMessageNotifier::relativePathMessage()
{
    return tr("The path input field contains a relative path");
}
QString InputMessageNotifier::rootPathMessage()
{
    return tr("The path input field contains a path from root");
}
QString InputMessageNotifier::networkPathMessage()
{
    return tr("The path input field contains a network path");
}

bool InputMessageNotifier::checkInvisibleSpace(const QString &str)
{
    return str.trimmed() != str;
}
bool InputMessageNotifier::checkDots(const QString &str)
{
    // TODO: Write regex
    return false;
}
bool InputMessageNotifier::checkFolderPath(const QString &str)
{
    // TODO: Write regex
    return false;
}
bool InputMessageNotifier::checkWindowsCompatibility(const QString &str)
{
    // TODO: Write regex
    return false;
}
bool InputMessageNotifier::checkRelativePath(const QString &str)
{
    // TODO: Write regex
    return false;
}
bool InputMessageNotifier::checkRootPath(const QString &str)
{
    // TODO: Write regex
    return false;
}
bool InputMessageNotifier::checkNetworkPath(const QString &str)
{
    // TODO: Write regex
    return false;
}

InputMessageNotifier::InputMessageNotifier(QWidget *parent) : QWidget(parent)
{
    this->m_layout = new QVBoxLayout;
    this->setLayout(this->m_layout);

    for (size_t i = 0; i < static_cast<size_t>(MessageNotifierType::MESSAGE_NOTIFIER_LENGTH); ++i) {
        this->m_messages[i] = new QLabel(this);

        this->m_messages[i]->setText(this->m_getMessages[i]());
        this->m_messages[i]->hide();
        this->m_messages[i]->setStyleSheet("background-color: rgb(249, 240, 107);\n"
                                           "border-radius: 10px;\n"
                                           "padding: 10px;");

        this->m_layout->addWidget(this->m_messages[i]);
    }
}

void InputMessageNotifier::addInstance(const QString& name)
{
    this->m_instances[name] = {};
}

void InputMessageNotifier::setInstanceDetection(const QString& name, MessageNotifierType type,
                                                bool detection)
{
    this->m_instances[name].trigger[static_cast<size_t>(type)] = detection;
    updateState();
}

void InputMessageNotifier::eraseInstance(const QString& name)
{
    this->m_instances.remove(name);
    updateState();
}

void InputMessageNotifier::updateInstance(const QString& name, const QString& content)
{
    auto &instance = this->m_instances[name];

    for (size_t i = 0; i < static_cast<size_t>(MessageNotifierType::MESSAGE_NOTIFIER_LENGTH); ++i) {
        instance.detected[i] = instance.trigger[i] && this->m_checkMessage[i](content);
    }

    instance.content = std::move(content);
    updateState();
}

void InputMessageNotifier::updateState()
{
    std::bitset<32> res{ 0 };

    for (const auto &instance : this->m_instances) {
        res |= instance.detected;
    }

    for (size_t i = 0; i < static_cast<size_t>(MessageNotifierType::MESSAGE_NOTIFIER_LENGTH); ++i) {
        if (res[i]) {
            this->m_messages[i]->show();
        } else {
            this->m_messages[i]->hide();
        }
    }
}

void InputMessageNotifier::retranslateUi()
{
    for (size_t i = 0; i < static_cast<size_t>(MessageNotifierType::MESSAGE_NOTIFIER_LENGTH); ++i) {
        this->m_messages[i]->setText(this->m_getMessages[i]());
    }
}

} // namespace preferences
