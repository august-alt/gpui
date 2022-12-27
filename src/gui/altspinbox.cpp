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

#include "altspinbox.h"

#include <QDebug>
#include <QMessageBox>

namespace gui
{
AltSpinBox::AltSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    connect(this, &AltSpinBox::fixToValidRange, this, &AltSpinBox::openMessageBox);
}

void AltSpinBox::fixup(QString &input) const
{
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
        int boundValue = qBound(minimum(), value, maximum());
        if (boundValue != value)
        {
            emit fixToValidRange(input);
        }
    }
    else
    {
        emit fixToValidRange(input);
    }
}

QValidator::State AltSpinBox::validate(QString &text, int &) const
{
    auto isOk  = false;
    auto value = text.toInt(&isOk, displayIntegerBase());

    if (isOk)
    {
        if (value >= minimum() && value <= maximum())
        {
            return QValidator::Acceptable;
        }
    }

    return QValidator::Intermediate;
}

void AltSpinBox::openMessageBox(const QString &wrongInput)
{
    int val = wrongInput.contains('-') ? this->minimum() : this->maximum();
    this->setValue(val);
    val == this->maximum() ? QMessageBox::information(nullptr,
                                                      QObject::tr("Information message"),
                                                      QObject::tr("Value: ") + wrongInput
                                                          + QObject::tr(" is greater than maximum allowed value of: ")
                                                          + QString::number(this->maximum())
                                                          + QObject::tr(". Maximum allowed value has been set."),
                                                      QMessageBox::Ok)
                           : QMessageBox::information(nullptr,
                                                      QObject::tr("Information message"),
                                                      QObject::tr("Value: ") + wrongInput
                                                          + QObject::tr(" is less than minimum allowed value of: ")
                                                          + QString::number(this->minimum())
                                                          + QObject::tr(". Minimum allowed value has been set."),
                                                      QMessageBox::Ok);
}

} // namespace gui
