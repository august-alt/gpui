#include "altspinbox.h"

#include <QDebug>

#include "infint.h"

namespace gui
{

void AltSpinBox::fixup(QString &input) const
{
    qWarning() << "AltSpinBox::fixup " << input << " " << minimum() << " " << maximum();

    QRegExp regExp("(-?)(\\+?)\\d+");
    auto isOk = regExp.exactMatch(input);
    auto value = InfInt(input.toStdString());
    if (isOk)
    {
        value = qBound(InfInt(minimum()), value, InfInt(maximum()));
        input = QString::number(value.toInt(), displayIntegerBase());
        qWarning() << "AltSpinBox::fixup " << input;
    }
    else
    {
        QSpinBox::fixup(input);
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
