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

#include "basemodelbuildertest.h"

#include "basemodelbuilder.h"

#include <string>

#include <QtGlobal>
#include <QtTest>
#include <QTimeZone>

#ifdef Q_OS_UNIX
#  include <time.h>
#endif

namespace {
const QString timestampFormat = "yyyy-MM-dd hh:mm:ss";

class TestModelBuilder : public preferences::BaseModelBuilder
{
public:
    using BaseModelBuilder::createDateOfChange;
    using BaseModelBuilder::createRootElement;
};

struct TestRootElement
{
    TestRootElement(const std::string &guidValue, const std::string &changedValue,
                    const std::string &descriptionValue)
        : guid(guidValue), changed(changedValue), description(descriptionValue)
    {
    }

    std::string guid;
    std::string changed;
    std::string description;
};

void updateSystemTimeZone()
{
#ifdef Q_OS_UNIX
    tzset();
#endif
}

} // namespace

namespace tests {
void BaseModelBuilderTest::initTestCase()
{
    originalTimeZone = qgetenv("TZ");
    hadOriginalTimeZone = qEnvironmentVariableIsSet("TZ");

    QVERIFY(qputenv("TZ", "Europe/Saratov"));
    updateSystemTimeZone();

    QVERIFY(QDateTime::currentDateTime().offsetFromUtc() != 0);
}

void BaseModelBuilderTest::cleanupTestCase()
{
    if (hadOriginalTimeZone) {
        qputenv("TZ", originalTimeZone);
    } else {
        qunsetenv("TZ");
    }
    updateSystemTimeZone();
}

void BaseModelBuilderTest::createDateOfChangeUsesUtc()
{
    TestModelBuilder builder;

    const QDateTime before = QDateTime::currentDateTimeUtc();
    const QString timestamp = QString::fromStdString(builder.createDateOfChange());
    const QDateTime after = QDateTime::currentDateTimeUtc();

    verifyUtcTimestamp(timestamp, before, after);
}

void BaseModelBuilderTest::createRootElementUsesUtc()
{
    TestModelBuilder builder;

    const QDateTime before = QDateTime::currentDateTimeUtc();
    const TestRootElement root = builder.createRootElement<TestRootElement>("test-guid");
    const QDateTime after = QDateTime::currentDateTimeUtc();

    QCOMPARE(QString::fromStdString(root.guid), QString("test-guid"));
    QCOMPARE(QString::fromStdString(root.description), QString());
    verifyUtcTimestamp(QString::fromStdString(root.changed), before, after);
}

void BaseModelBuilderTest::verifyUtcTimestamp(const QString &timestamp, const QDateTime &before,
                                              const QDateTime &after)
{
    QVERIFY(timestamp.size() == 19);

    QDateTime parsedTimestamp = QDateTime::fromString(timestamp, timestampFormat);
    QVERIFY(parsedTimestamp.isValid());
    QCOMPARE(parsedTimestamp.toString(timestampFormat), timestamp);

    parsedTimestamp.setTimeZone(QTimeZone::utc());
    QVERIFY(parsedTimestamp >= before.addSecs(-1));
    QVERIFY(parsedTimestamp <= after);
}

} // namespace tests

QTEST_APPLESS_MAIN(tests::BaseModelBuilderTest)
