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

class InputDetector 
{
public:
    virtual bool detect(const QString& input) = 0;
    virtual ~InputDetector() = default;
};

class WhitespaceDetector: public InputDetector 
{
public:
    bool detect(const QString& input) override;
};

class InputMessageNotifier : public QWidget 
{
public:

    Q_OBJECT

public: 

    enum class MessageLevel
    {
        Warning,
        Error,
    };

private:

    typedef struct DetectElement
    {
    public:
        DetectElement(QWidget* parent, QLayout* layout, const QString& message, MessageLevel level);
        DetectElement(DetectElement&& element);
        DetectElement& operator=(DetectElement&& element);

        void detect();
        void undetect();

        bool detected();
        MessageLevel level();
        void setMessage(const QString& message);

        ~DetectElement();
    private:
    DetectElement(const DetectElement&) = delete;
    void operator=(const DetectElement&) = delete;

    private:
        bool m_detected{false};
        QLabel* m_label{nullptr};
        MessageLevel m_level{MessageLevel::Warning};
    } DetectElement;

    typedef std::unordered_map<size_t, DetectElement> InputInstance;

public:

    InputMessageNotifier(QWidget* widget);

    /**
     * @brief add detector.
     * @param detector Detector.
     * @return Detector id.
     */
    size_t addDetector(std::unique_ptr<InputDetector> detector);

    /**
     * @brief remove detector.
     * @param id Detector id.
     */
    void removeDetector(size_t id);

    /**
     * @brief registrate input.
     * @param name Name of input to be registrated.
     */
    void addInput(const QString& name);

    /**
     * @brief unregistrate input.
     * @param name Name of input to be unregistrated.
     */
    void removeInput(const QString& name);

    /**
     * @brief update input content and validate it.
     * @param name Name of registrated input.
     * @param input Input content.
     */
    void updateInput(const QString& name, const QString& input);

    /**
     * @brief update input content and validate it.
     * @param name Name of registrated input.
     * @param detector Detector id.
     * @param message MessageNotifier message on detect.
     */
    void setMessage(const QString& name, size_t detector, const QString& message);

    /**
     * @brief attach detector to input.
     * @param name Name of registrated input.
     * @param detector Detector id.
     * @param level Message level(visual and see hasAnyError() and hasAnyWarning())
     */
    void attachDetector(const QString& name, size_t detector, const QString& message, MessageLevel level = MessageLevel::Warning);

    /**
     * @return true, if any error has been detected(on any input).
     */
    bool hasAnyError();

    /**
     * @return true, if any warning has been detected(on any input).
     */
    bool hasAnyWarning();

    ~InputMessageNotifier();

private:
    InputMessageNotifier(const InputMessageNotifier&) = delete;
    void operator=(const InputMessageNotifier&) = delete;

    void incCounter(MessageLevel level);
    void decCounter(MessageLevel level);

private:

    std::unordered_map<size_t, std::unique_ptr<InputDetector>> m_detectors{};
    std::unordered_map<QString, InputInstance> m_instances{};

    size_t m_errorCount{0};
    size_t m_warningCount{0};

    size_t m_nextDetector{0};

    QLayout* m_layout{nullptr};
};

} // namespace preferences

#endif // GPUI_INPUTMESSAGENOTIFIER_H
