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

#ifndef GPUI_INPUTMESSAGENOTIFIER_H
#define GPUI_INPUTMESSAGENOTIFIER_H

#include <QtCore>
#include <QtWidgets>
#include <bits/stdc++.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class InputMessageNotifier;
}
QT_END_NAMESPACE

namespace preferences {

class InputMessageNotifier : public QWidget
{
public:
    Q_OBJECT

public:
    typedef enum class MessageNotifierType {
        // Warns if one of the following regular expressions is executed for the entire string
        INVISIBLE_SPACE = 0, // " .*|.* |.+ {2}.+"
        DOTS = 1, // ".+\.{1,2}.*"
        FOLDER_PATH = 2, // "*./"
        WINDOWS_COMPATIBILITY = 3, // TODO: write regex
        RELATIVE_PATH = 4, // TODO: write regex (for linux and windows)
        ROOT_PATH = 5, // TODO: write regex (for linux and windows)
        NETWORK_PATH = 6, // TODO: write regex (for linux and windows)
        MESSAGE_NOTIFIER_LENGTH,
    } MessageNotifierType;

private:
    typedef std::bitset<32> MessageNotifierInfo;

    typedef struct MessageInstance
    {
        MessageNotifierInfo trigger{0};
        MessageNotifierInfo detected{0};
        QString content{};
    } MessageInstance;

    typedef QString (*MessageStringFunc)();
    typedef bool (*CheckStringFunc)(const QString &str);

private:
    static QString invisibleSpaceMessage();
    static QString dotsMessage();
    static QString folderPathMessage();
    static QString windowsCompatibilityMessage();
    static QString relativePathMessage();
    static QString rootPathMessage();
    static QString networkPathMessage();

    static bool checkInvisibleSpace(const QString &str);
    static bool checkDots(const QString &str);
    static bool checkFolderPath(const QString &str);
    static bool checkWindowsCompatibility(const QString &str);
    static bool checkRelativePath(const QString &str);
    static bool checkRootPath(const QString &str);
    static bool checkNetworkPath(const QString &str);

    void updateState();

private:
    static constexpr std::array<MessageStringFunc,
                                static_cast<size_t>(MessageNotifierType::MESSAGE_NOTIFIER_LENGTH)>
            m_getMessages = { &invisibleSpaceMessage, &dotsMessage,
                              &folderPathMessage,     &windowsCompatibilityMessage,
                              &relativePathMessage,   &rootPathMessage,
                              &networkPathMessage };
    static constexpr std::array<CheckStringFunc,
                                static_cast<size_t>(MessageNotifierType::MESSAGE_NOTIFIER_LENGTH)>
            m_checkMessage = { &checkInvisibleSpace,       &checkDots,         &checkFolderPath,
                               &checkWindowsCompatibility, &checkRelativePath, &checkRootPath,
                               &checkNetworkPath };

public:
    explicit InputMessageNotifier(QWidget *parent = nullptr);

    void addInstance(const QString& name);
    void setInstanceDetection(const QString& name, MessageNotifierType type, bool detection = true);
    void eraseInstance(const QString& name);
    void updateInstance(const QString& name, const QString &content);

    void retranslateUi();

private:
    InputMessageNotifier(const InputMessageNotifier &) = delete; // copy ctor
    InputMessageNotifier(InputMessageNotifier &&) = delete; // move ctor
    InputMessageNotifier &operator=(const InputMessageNotifier &) = delete; // copy assignment
    InputMessageNotifier &operator=(InputMessageNotifier &&) = delete; // move assignment

private:
    QMap<QString, MessageInstance> m_instances{};
    QVBoxLayout* m_layout{ nullptr };
    std::array<QLabel *, static_cast<size_t>(MessageNotifierType::MESSAGE_NOTIFIER_LENGTH)>
            m_messages{ nullptr };
};

} // namespace preferences

#endif // GPUI_INPUTMESSAGENOTIFIER_H
