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

class IInputMessageDetector : public QObject
{
public:
    Q_OBJECT

public:
    virtual bool detect(const QString& input) = 0;
    virtual QWidget* raw() = 0;
    virtual const QString& message() = 0;
    virtual ~IInputMessageDetector() = 0;
};

class InputMessageWhitespaceDetector : public IInputMessageDetector
{
public:
    Q_OBJECT

public:
    bool detect(const QString& input) override;
    QWidget* raw() override;
    const QString& message() override;
};

class InputMessageNotifier : public QWidget
{
public:
    Q_OBJECT
private:

typedef struct MessageInstance 
{
    std::unordered_set<size_t> mask{};
    std::unordered_set<size_t> detected{};
    QString content;
} MessageInstance;

public:
    explicit InputMessageNotifier(QWidget *parent = nullptr);

    size_t addDetector(std::unique_ptr<IInputMessageDetector> detector);

    void addInstance(const QString& name);
    void eraseInstance(const QString& name);
    void setInstanceDetection(const QString& name, size_t type, bool detect = true);
    void updateInstance(const QString& name, const QString &content);

    void retranslateUi();

private:
    InputMessageNotifier(const InputMessageNotifier &) = delete; // copy ctor
    InputMessageNotifier(InputMessageNotifier &&) = delete; // move ctor
    InputMessageNotifier &operator=(const InputMessageNotifier &) = delete; // copy assignment
    InputMessageNotifier &operator=(InputMessageNotifier &&) = delete; // move assignment'

    void updateState();

private:
    QVBoxLayout* m_layout{nullptr};
    std::vector<bool> triggered{};
    std::unordered_map<QString, MessageInstance> m_instances{};
    std::unordered_map<size_t, std::unique_ptr<IInputMessageDetector>> m_detectors{};
    size_t m_next_detector = 0;
};

} // namespace preferences

#endif // GPUI_INPUTMESSAGENOTIFIER_H
