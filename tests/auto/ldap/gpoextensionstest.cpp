// Copyright (C) 2026 BaseALT Ltd.
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ldap/gpoextensions_p.h"

#include <QTest>

namespace
{
const QString machineAttribute = QStringLiteral("gPCMachineExtensionNames");
const QString userAttribute = QStringLiteral("gPCUserExtensionNames");
const QString dn = QStringLiteral("CN={11111111-1111-1111-1111-111111111111},CN=Policies,DC=test");
const QString first = QStringLiteral("{11111111-1111-1111-1111-111111111111}");
const QString second = QStringLiteral("{AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA}");
const QString third = QStringLiteral("{FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF}");

QString registration(const QString &cse, const QString &tool)
{
    return '[' + cse + tool + ']';
}

struct Directory
{
    QHash<QString, QString> values;
    QStringList calls;
    int version = 0;
    int failedWrite = 0;
    bool failedRead = false;
    bool additionalResult = false;
    QString returnedDn = dn;

    QHash<QString, AdObject> search(const QString &path,
                                    SearchScope scope,
                                    const QString &filter,
                                    const QList<QString> &attributes)
    {
        calls << "read";
        QHash<QString, AdObject> objects;
        if (!failedRead && path == dn && scope == SearchScope_Object && filter.isEmpty()
            && attributes == QList<QString>{machineAttribute, userAttribute})
        {
            QHash<QString, QList<QByteArray>> data;
            for (auto entry = values.cbegin(); entry != values.cend(); ++entry)
            {
                data.insert(entry.key(), {entry.value().toUtf8()});
            }
            AdObject object;
            object.load(returnedDn, data);
            objects.insert(returnedDn, object);
            if (additionalResult)
            {
                objects.insert("CN=Other," + dn, object);
            }
        }
        return objects;
    }

    bool attribute_replace_string(const QString &path, const QString &attribute, const QString &value)
    {
        calls << attribute;
        if (path != dn || calls.size() - 1 == failedWrite)
        {
            return false;
        }
        for (auto entry = values.begin(); entry != values.end();)
        {
            if (entry.key().compare(attribute, Qt::CaseInsensitive) == 0)
            {
                entry = values.erase(entry);
            }
            else
            {
                ++entry;
            }
        }
        values[attribute] = value;
        return true;
    }

    bool attribute_replace_int(const QString &path, const QString &attribute, int value)
    {
        calls << attribute;
        if (path != dn || calls.size() - 1 == failedWrite)
        {
            return false;
        }
        version = value;
        return true;
    }
};
}

class GpoExtensionsTest : public QObject
{
    Q_OBJECT

private slots:
    void preservesExtensionsAndTools()
    {
        Directory directory;
        directory.values[machineAttribute] = registration(third, first) + registration(first, third);
        directory.values[userAttribute] = registration(second.toLower(), third.toLower());
        const QString required = registration(second, first) + registration(first, second)
            + registration(second.toLower(), first.toLower());

        QVERIFY(ldap::detail::setGpoExtensions(directory, dn, required, registration(second, first), 524292));
        QCOMPARE(directory.values[machineAttribute], registration(first, second + third)
                     + registration(second, first) + registration(third, first));
        QCOMPARE(directory.values[userAttribute], registration(second, first + third));
        QCOMPARE(directory.version, 524292);
        QCOMPARE(directory.calls, QStringList({"read", machineAttribute, userAttribute, "versionNumber"}));

        const auto previous = directory.values;
        QVERIFY(ldap::detail::setGpoExtensions(directory, dn, required, registration(second, first), 589829));
        QCOMPARE(directory.values, previous);
    }

    void addsFirstExtensions()
    {
        Directory directory;
        QVERIFY(ldap::detail::setGpoExtensions(directory, dn, registration(second, first), QString(), 1));
        QCOMPARE(directory.values[machineAttribute], registration(second, first));
        QCOMPARE(directory.values[userAttribute], QString());
        QCOMPARE(directory.version, 1);
    }

    void failedReadDoesNotWrite()
    {
        Directory directory;
        directory.failedRead = true;
        QVERIFY(!ldap::detail::setGpoExtensions(directory, dn, registration(second, first), QString(), 1));
        QCOMPARE(directory.calls, QStringList({"read"}));
        QVERIFY(directory.values.isEmpty());
    }

    void acceptsCanonicalDnCase()
    {
        Directory directory;
        directory.returnedDn = dn.toLower();
        QVERIFY(directory.returnedDn != dn);
        directory.values[machineAttribute] = registration(third, first);
        QVERIFY(ldap::detail::setGpoExtensions(directory, dn, registration(second, first), QString(), 1));
        QCOMPARE(directory.values[machineAttribute], registration(second, first) + registration(third, first));
        QCOMPARE(directory.version, 1);
    }

    void invalidSearchResultsDoNotWrite()
    {
        Directory missingDn;
        missingDn.returnedDn.clear();
        QVERIFY(!ldap::detail::setGpoExtensions(missingDn, dn, registration(second, first), QString(), 1));
        QCOMPARE(missingDn.calls, QStringList({"read"}));

        Directory multipleResults;
        multipleResults.additionalResult = true;
        QVERIFY(!ldap::detail::setGpoExtensions(multipleResults, dn, registration(second, first), QString(), 1));
        QCOMPARE(multipleResults.calls, QStringList({"read"}));
    }

    void acceptsAttributeNameCase()
    {
        Directory directory;
        directory.values[machineAttribute.toLower()] = registration(third, first);
        directory.values[userAttribute.toUpper()] = registration(first, third);
        QVERIFY(ldap::detail::setGpoExtensions(directory, dn, registration(second, first), QString(), 1));
        QCOMPARE(directory.values[machineAttribute], registration(second, first) + registration(third, first));
        QCOMPARE(directory.values[userAttribute], registration(first, third));
    }

    void malformedMetadata_data()
    {
        QTest::addColumn<QString>("malformed");
        QTest::newRow("unbracketed") << first + second;
        QTest::newRow("no-tool") << '[' + first + ']';
        QTest::newRow("bad-guid") << registration("{bad}", second);
        QTest::newRow("leading-text") << "x" + registration(first, second);
        QTest::newRow("trailing-text") << registration(first, second) + "x";
        QTest::newRow("empty-group") << "[]";
    }

    void malformedMetadata()
    {
        QFETCH(QString, malformed);
        for (const QString &attribute : {machineAttribute, userAttribute})
        {
            Directory directory;
            directory.values[attribute] = malformed;
            const auto previous = directory.values;
            QVERIFY(!ldap::detail::setGpoExtensions(directory, dn, registration(second, first), QString(), 1));
            QCOMPARE(directory.values, previous);
            QCOMPARE(directory.calls, QStringList({"read"}));
        }
    }

    void stopsAfterWriteFailure()
    {
        for (int failedWrite : {1, 2, 3})
        {
            Directory directory;
            directory.failedWrite = failedWrite;
            QVERIFY(!ldap::detail::setGpoExtensions(directory, dn, registration(second, first), QString(), 1));
            QCOMPARE(directory.calls.size(), failedWrite + 1);
            QCOMPARE(directory.version, 0);
        }
    }
};

QTEST_APPLESS_MAIN(GpoExtensionsTest)
#include "gpoextensionstest.moc"
