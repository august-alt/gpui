// Copyright (C) 2026 BaseALT Ltd.
// SPDX-License-Identifier: GPL-2.0-or-later

#include "core/pluginstorage.h"
#include "core/snapinmanager.h"
#include "core/translatorstorage.h"
#include "gui/commandlineoptions.h"
#include "gui/mainwindow.h"
#include "plugins/scripts/scriptsdialog.h"
#include "plugins/scripts/scriptssnapin.h"

#include <QDir>
#include <QFile>
#include <QPushButton>
#include <QSettings>
#include <QTemporaryDir>
#include <QTest>
#include <QTextStream>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QStringConverter>
#endif

static int policyVersion(const QString &path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        for (const QByteArray &line : file.readAll().split('\n'))
        {
            if (line.startsWith("Version="))
            {
                return line.mid(8).trimmed().toInt();
            }
        }
    }
    return -1;
}

class ScriptsPolicyVersionTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        QSettings::setDefaultFormat(QSettings::IniFormat);
        gpui::PluginStorage::instance()->loadPluginDirectory(TEST_PLUGIN_DIR);
    }

    void saveChanges_data()
    {
        QTest::addColumn<bool>("user");
        QTest::addColumn<bool>("startup");
        QTest::addColumn<int>("button");

        for (bool user : {false, true})
        {
            for (bool startup : {false, true})
            {
                for (int button : {QDialogButtonBox::Apply, QDialogButtonBox::Ok})
                {
                    const QByteArray name = QByteArray(user ? "user-" : "machine-")
                        + (startup ? "start-" : "stop-") + (button == QDialogButtonBox::Apply ? "apply" : "ok");
                    QTest::newRow(name.constData()) << user << startup << button;
                }
            }
        }
    }

    void saveChanges()
    {
        QFETCH(bool, user);
        QFETCH(bool, startup);
        QFETCH(int, button);

        QTemporaryDir directory;
        QVERIFY(directory.isValid());
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, directory.path());
        const QString gptPath = directory.filePath("gpt.ini");
        {
            QFile gpt(gptPath);
            QVERIFY(gpt.open(QIODevice::WriteOnly));
            gpt.write("[General]\nVersion=458755\n");
        }

        const QString part = user ? "User" : "Machine";
        const QString action = user ? (startup ? "Logon" : "Logoff") : (startup ? "Startup" : "Shutdown");
        const QString scriptsDirectory = directory.filePath(part + "/Scripts");
        QVERIFY(QDir().mkpath(scriptsDirectory));
        const QString scriptsPath = scriptsDirectory + "/scripts.ini";
        {
            QFile scripts(scriptsPath);
            QVERIFY(scripts.open(QIODevice::WriteOnly));
            QTextStream stream(&scripts);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            stream.setEncoding(QStringConverter::Utf16LE);
#else
            stream.setCodec("UTF-16LE");
#endif
            stream.setGenerateByteOrderMark(true);
            stream << '[' << action << "]\n0CmdLine=/usr/bin/login-log\n0Parameters=hello local\n";
        }

        gpui::CommandLineOptions options;
        options.path = directory.path();
        options.policyBundle = directory.path();
        gpui::SnapInManager manager;
        TranslatorStorage translations;
        gpui::MainWindow window(options, &manager, &translations);
        scripts_plugin::ScriptsSnapIn snapIn;
        snapIn.onInitialize(&window);
        snapIn.onDataLoad(directory.path().toStdString(), "en-US");

        auto save = [&]() -> bool {
            scripts_plugin::ScriptsDialog dialog;
            dialog.setModels(user ? snapIn.d->userScriptsModel.get() : snapIn.d->machineScriptsModel.get(),
                             user ? snapIn.d->userPowerScriptsModel.get() : snapIn.d->machinePowerScriptsModel.get(),
                             startup, user);
            auto *buttons = dialog.findChild<QDialogButtonBox *>("buttonBox");
            auto *saveButton = buttons ? buttons->button(static_cast<QDialogButtonBox::StandardButton>(button)) : nullptr;
            if (!saveButton || !QObject::connect(&dialog, SIGNAL(saveDataSignal()), snapIn.d, SLOT(saveData())))
            {
                return false;
            }
            saveButton->click();
            return true;
        };

        QVERIFY(save());
        QCOMPARE(policyVersion(gptPath), (8 << 16) + 4);
        {
            QFile scripts(scriptsPath);
            QVERIFY(scripts.open(QIODevice::ReadOnly));
            QTextStream stream(&scripts);
            QVERIFY(stream.readAll().contains("/usr/bin/login-log"));
        }

        auto *model = user ? snapIn.d->userScriptsModel.get() : snapIn.d->machineScriptsModel.get();
        model->clear();
        QVERIFY(save());
        QCOMPARE(policyVersion(gptPath), (9 << 16) + 5);
        QFile scripts(scriptsPath);
        QVERIFY(scripts.open(QIODevice::ReadOnly));
        QTextStream stream(&scripts);
        QVERIFY(!stream.readAll().contains("login-log"));

        scripts_plugin::ScriptsDialog cancelled;
        QVERIFY(QObject::connect(&cancelled, SIGNAL(saveDataSignal()), snapIn.d, SLOT(saveData())));
        QVERIFY(QObject::connect(&cancelled, SIGNAL(reloaddataSignal()), snapIn.d, SLOT(reloadData())));
        auto *buttons = cancelled.findChild<QDialogButtonBox *>("buttonBox");
        QVERIFY(buttons);
        QVERIFY(buttons->button(QDialogButtonBox::Cancel));
        buttons->button(QDialogButtonBox::Cancel)->click();
        QCOMPARE(policyVersion(gptPath), (9 << 16) + 5);
    }
};

QTEST_MAIN(ScriptsPolicyVersionTest)
#include "scriptspolicyversiontest.moc"
