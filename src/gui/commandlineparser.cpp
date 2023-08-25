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

#include "commandlineparser.h"

#include <iostream>
#include <memory>

#include <QCommandLineParser>
#include <QLibraryInfo>
#include <QTranslator>
#include <QUuid>

namespace gpui
{
class CommandLineParserPrivate
{
public:
    QApplication &application;
    std::unique_ptr<QCommandLineParser> parser;

    CommandLineParserPrivate(QApplication &currentApplication)
        : application(currentApplication)
        , parser(std::make_unique<QCommandLineParser>())
    {}

private:
    CommandLineParserPrivate(const CommandLineParserPrivate &) = delete;            // copy ctor
    CommandLineParserPrivate(CommandLineParserPrivate &&)      = delete;            // move ctor
    CommandLineParserPrivate &operator=(const CommandLineParserPrivate &) = delete; // copy assignment
    CommandLineParserPrivate &operator=(CommandLineParserPrivate &&) = delete;      // move assignment
};

CommandLineParser::CommandLineParser(QApplication &application)
    : d(new CommandLineParserPrivate(application))
{}

CommandLineParser::~CommandLineParser()
{
    delete d;
}

CommandLineParser::CommandLineParseResult CommandLineParser::parseCommandLine(CommandLineOptions *options,
                                                                              QString *errorMessage)
{
    QLocale locale;
    std::unique_ptr<QTranslator> qtTranslator = std::make_unique<QTranslator>();
    qtTranslator->load(locale, "gui", "_", ":/");
    std::unique_ptr<QTranslator> qtTranslator2 = std::make_unique<QTranslator>();
    qtTranslator2->load(QString("qt_").append(QLocale::system().name()),
                        QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QCoreApplication::installTranslator(qtTranslator.get());
    QCoreApplication::installTranslator(qtTranslator2.get());

    const QCommandLineOption pathOption("p", QObject::tr("The full path of policy to edit."), QObject::tr("path"));
    const QCommandLineOption bundleOption("b",
                                          QObject::tr("The full path of policy bundle to load."),
                                          QObject::tr("path"));
    const QCommandLineOption nameOption("n",
                                        QObject::tr("This options left for compatibility with ADMC. "
                                                    "Currently it does nothing."),
                                        QObject::tr("name"));
    const QCommandLineOption helpOption(QStringList()
#ifdef Q_OS_WIN
                                            << QStringLiteral("?")
#endif
                                            << QStringLiteral("h") << QStringLiteral("help"),
                                        QObject::tr("Displays help on commandline options."));

#ifdef QT_DEBUG
#define CONSOLE_LOG_LEVEL_DEFAULT "debug"
#define SYSLOG_LOG_LEVEL_DEFAULT "none"
#define FILE_LOG_LEVEL_DEFAULT "debug"
#else
#define CONSOLE_LOG_LEVEL_DEFAULT "none"
#define SYSLOG_LOG_LEVEL_DEFAULT "none"
#define FILE_LOG_LEVEL_DEFAULT "warning"
#endif

    const QCommandLineOption consoleLogLevelOpion("log-console",
                                                  QObject::tr("Set log level for console. ") + QObject::tr("Default")
                                                      + ": \"" CONSOLE_LOG_LEVEL_DEFAULT "\".",
                                                  QObject::tr("level"),
                                                  CONSOLE_LOG_LEVEL_DEFAULT);
    const QCommandLineOption syslogLogLevelOpion("log-syslog",
                                                 QObject::tr("Set log level for syslog. ") + QObject::tr("Default")
                                                     + ": \"" SYSLOG_LOG_LEVEL_DEFAULT "\".",
                                                 QObject::tr("level"),
                                                 SYSLOG_LOG_LEVEL_DEFAULT);
    const QCommandLineOption fileLogLevelOpion("log-file",
                                               QObject::tr("Set log level for file in ~/.local/share/gpui/. ")
                                                   + QObject::tr("Default") + ": \"" FILE_LOG_LEVEL_DEFAULT "\".",
                                               QObject::tr("level"),
                                               FILE_LOG_LEVEL_DEFAULT);

    d->parser->setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    d->parser->addOption(pathOption);
    d->parser->addOption(bundleOption);
    d->parser->addOption(helpOption);
    d->parser->addOption(nameOption);
    d->parser->addOption(consoleLogLevelOpion);
    d->parser->addOption(syslogLogLevelOpion);
    d->parser->addOption(fileLogLevelOpion);

    const QCommandLineOption versionOption = d->parser->addVersionOption();

    if (!d->parser->parse(d->application.arguments()))
    {
        *errorMessage = d->parser->errorText();
        return CommandLineError;
    }

    if (d->parser->isSet(versionOption))
    {
        return CommandLineVersionRequested;
    }

    if (d->parser->isSet(helpOption))
    {
        return CommandLineHelpRequested;
    }

    if (d->parser->isSet(pathOption))
    {
        const QString path = d->parser->value(pathOption);
        options->path      = path;

        if (options->path.isNull() || options->path.isEmpty())
        {
            *errorMessage = QObject::tr("Bad policy path: ") + path;
            return CommandLineError;
        }
    }

    if (d->parser->isSet(bundleOption))
    {
        const QString path    = d->parser->value(bundleOption);
        options->policyBundle = path;

        if (options->policyBundle.isNull() || options->policyBundle.isEmpty())
        {
            *errorMessage = QObject::tr("Bad policy path: ") + path;
            return CommandLineError;
        }
    }

    if (!handleLoggerOption(consoleLogLevelOpion, options->consoleLogLevel, errorMessage)
        || !handleLoggerOption(syslogLogLevelOpion, options->syslogLogLevel, errorMessage)
        || !handleLoggerOption(fileLogLevelOpion, options->fileLogLevel, errorMessage))
    {
        return CommandLineError;
    }

    return CommandLineOk;
}

bool CommandLineParser::handleLoggerOption(const QCommandLineOption &option, QtMsgType &result, QString *errorMessage)
{
    if (d->parser->isSet(option))
    {
        const QString logString = d->parser->value(option);

        if (logString == "none")
        {
            result = LOG_LEVEL_DISABLED;
        }
        else if (logString == "debug")
        {
            result = QtDebugMsg;
        }
        else if (logString == "info")
        {
            result = QtInfoMsg;
        }
        else if (logString == "warning")
        {
            result = QtWarningMsg;
        }
        else if (logString == "critical")
        {
            result = QtCriticalMsg;
        }
        else if (logString == "fatal")
        {
            result = QtFatalMsg;
        }
        else
        {
            *errorMessage = QObject::tr("Bad log level: ") + logString;
            return false;
        }
    }

    return true;
}

void CommandLineParser::showHelp() const
{
    std::cerr << qPrintable(d->parser->helpText());
}

void CommandLineParser::showVersion() const
{
    d->parser->showVersion();
}

} // namespace gpui
