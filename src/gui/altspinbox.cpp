#include "altspinbox.h"
#include <QDebug>

namespace gui
{

void AltSpinBox::fixup(QString &input) const
{
    qWarning() << "AltSpinBox::fixup " << input << " " << minimum() << " " << maximum();

    QRegExp regExp("(-?)(\\+?)\\d+");
    if (!regExp.exactMatch(input))
    {
        QSpinBox::fixup(input);
        return;
    }
    bool isOk = false;
    int value = input.toInt(&isOk);
    if (isOk)
    {
        value = qBound(minimum(), value, maximum());
        input = QString::number(value, displayIntegerBase());
        qWarning() << "AltSpinBox::fixup " << input;
    }
    else
    {
        emit fixToValidRange(input);
    }
}

QValidator::State AltSpinBox::validate(QString &text, int &) const
{
    auto isOk = false;
    auto value = text.toInt(&isOk, displayIntegerBase());

    qWarning() << "AltSpinBox::validate " << text << " " << isOk << " " << value;

    if (isOk)
    {
        if (value >= minimum() && value <= maximum())
        {
            return QValidator::Acceptable;
        }
    }

    return QValidator::Intermediate;
}

}
