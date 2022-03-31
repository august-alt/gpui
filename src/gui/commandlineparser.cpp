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

#include <memory>

#include <QUuid>
#include <QCommandLineParser>
#include <QTranslator>
#include <QLibraryInfo>

namespace gpui
{

class CommandLineParserPrivate
{
public:
    QApplication& application;
    std::unique_ptr<QCommandLineParser> parser;

    CommandLineParserPrivate(QApplication &currentApplication)
        : application(currentApplication)
        , parser(std::make_unique<QCommandLineParser>())
    {
    }

private:
    CommandLineParserPrivate(const CommandLineParserPrivate&)            = delete;   // copy ctor
    CommandLineParserPrivate(CommandLineParserPrivate&&)                 = delete;   // move ctor
    CommandLineParserPrivate& operator=(const CommandLineParserPrivate&) = delete;   // copy assignment
    CommandLineParserPrivate& operator=(CommandLineParserPrivate&&)      = delete;   // move assignment
};

CommandLineParser::CommandLineParser(QApplication &application)
    : d(new CommandLineParserPrivate(application))
{
}

CommandLineParser::~CommandLineParser()
{
    delete d;
}

CommandLineParser::CommandLineParseResult CommandLineParser::parseCommandLine(CommandLineOptions *options, QString *errorMessage)
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
    const QCommandLineOption bundleOption("b", QObject::tr("The full path of policy bundle to load."), QObject::tr("path"));
    const QCommandLineOption nameOption("n", QObject::tr("The name of a policy to display."), QObject::tr("name"));
    const QCommandLineOption helpOption(QStringList()
    #ifdef Q_OS_WIN
                   << QStringLiteral("?")
    #endif
                   << QStringLiteral("h")
                   << QStringLiteral("help"), QObject::tr("Displays help on commandline options."));

    d->parser->setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    d->parser->addOption(pathOption);
    d->parser->addOption(bundleOption);
    d->parser->addOption(helpOption);
    d->parser->addOption(nameOption);

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
        options->path = path;

        if (options->path.isNull() || options->path.isEmpty())
        {
            *errorMessage = QObject::tr("Bad policy path: ") + path;
            return CommandLineError;
        }
    }

    if (d->parser->isSet(bundleOption))
    {
        const QString path = d->parser->value(bundleOption);
        options->policyBundle = path;

        if (options->policyBundle.isNull() || options->policyBundle.isEmpty())
        {
            *errorMessage = QObject::tr("Bad policy path: ") + path;
            return CommandLineError;
        }
    }

    if (d->parser->isSet(nameOption))
    {
        const QString name = d->parser->value(nameOption);
        options->policyName = name;

        if (options->policyName.isNull() || options->policyName.isEmpty())
        {
            *errorMessage = QObject::tr("Bad policy name: ") + name;
            return CommandLineError;
        }
    }


    return CommandLineOk;
}

void CommandLineParser::showHelp() const
{
    d->parser->showHelp();
}

void CommandLineParser::showVersion() const
{
    d->parser->showVersion();
}

}
