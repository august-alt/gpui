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

#include <QtWidgets>
#include <QtCore>

QT_BEGIN_NAMESPACE
namespace Ui { class InputMessageNotifier; }
QT_END_NAMESPACE

namespace preferences
{

class InputMessageNotifier : public QWidget
{
public:
    Q_OBJECT
private:
    typedef struct MessageNotifierInfoFields
    {
        bool invisibleSpace       : 1;  // example: ` /var/hello`
        bool dots                 : 1;  // example: `/var/./hello` | `/var/../hello`
        bool folderPath           : 1;  // example: `/var/hello/`
        bool windowsCompatibility : 1;  // any path, that can't be Windows path or UNC path.
        bool localPath            : 1;  // any path, that can't be local: `var/hello`
        bool rootPath             : 1;  // any path, that can't be root: `/var/hello`
        bool networkPath          : 1;  // any path, that can't be network: `smb://SMB.DOMAIN.TEST/Test/path`
    } MessageNotifierInfoFields;

public:
    typedef union MessageNotifierInfo
    {
        MessageNotifierInfoFields fields;
        uint32_t mask{0};
    } MessageNotifierInfo;

    typedef struct MessageInstance 
    {
        MessageNotifierInfo trigger;
        MessageNotifierInfo detected;
    } MessageInstance;

public:
    explicit InputMessageNotifier(QWidget* parent = nullptr);

private:
    InputMessageNotifier(const InputMessageNotifier&)            = delete;   // copy ctor
    InputMessageNotifier(InputMessageNotifier&&)                 = delete;   // move ctor
    InputMessageNotifier& operator=(const InputMessageNotifier&) = delete;   // copy assignment
    InputMessageNotifier& operator=(InputMessageNotifier&&)      = delete;   // move assignment

private:
    QMap<QString, MessageInstance> m_instances{};
    QStaticArrayData<QWidget*, 7> m_messages{};
}; 

}

#endif //GPUI_INPUTMESSAGENOTIFIER_H
