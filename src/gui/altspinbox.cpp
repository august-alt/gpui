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

#include "../core/logger/log.h"
#include <QMessageBox>

namespace gui
{
AltSpinBox::AltSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    connect(this, &AltSpinBox::fixStringInput, this, &AltSpinBox::onFixStringInput);
    connect(this, &AltSpinBox::fixToValidRange, this, &AltSpinBox::onFixToValidRange);
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
            emit fixToValidRange(value, boundValue);
        }
    }
    else
    {
        emit fixStringInput(input);
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

void AltSpinBox::onFixStringInput(const QString &wrongInput)
{
    int value = wrongInput.contains('-') ? this->minimum() : this->maximum();
    this->setValue(value);
    openMessageBox(wrongInput, value == this->maximum());
}

void AltSpinBox::onFixToValidRange(const int currentValue, const int boundValue)
{
    bool isMaximum = boundValue == maximum();
    this->setValue(boundValue);
    openMessageBox(QString::number(currentValue, displayIntegerBase()), isMaximum);
}

void AltSpinBox::openMessageBox(const QString &value, bool isMaximum)
{
    if (isMaximum)
    {
        QMessageBox::information(nullptr,
                                 QObject::tr("Information message"),
                                 QObject::tr("Value: ") + value
                                     + QObject::tr(" is greater than maximum allowed value of: ")
                                     + QString::number(this->maximum())
                                     + QObject::tr(". Maximum allowed value has been set."),
                                 QMessageBox::Ok);
    }
    else
    {
        QMessageBox::information(nullptr,
                                 QObject::tr("Information message"),
                                 QObject::tr("Value: ") + value + QObject::tr(" is less than minimum allowed value of: ")
                                     + QString::number(this->minimum())
                                     + QObject::tr(". Minimum allowed value has been set."),
                                 QMessageBox::Ok);
    }
}

} // namespace gui
