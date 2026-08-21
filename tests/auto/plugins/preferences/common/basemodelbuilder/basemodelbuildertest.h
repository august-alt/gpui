/***********************************************************************************************************************
**
** Copyright (C) 2026 BaseALT Ltd. <org@basealt.ru>
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

#ifndef GPUI_BASE_MODEL_BUILDER_TEST_H
#define GPUI_BASE_MODEL_BUILDER_TEST_H

#include <QByteArray>
#include <QDateTime>
#include <QObject>

namespace tests {
class BaseModelBuilderTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void createDateOfChangeUsesUtc();
    void createRootElementUsesUtc();

private:
    void verifyUtcTimestamp(const QString &timestamp, const QDateTime &before,
                            const QDateTime &after);

    QByteArray originalTimeZone;
    bool hadOriginalTimeZone = false;
};

} // namespace tests

#endif // GPUI_BASE_MODEL_BUILDER_TEST_H
