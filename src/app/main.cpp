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

#include "../core/compositesnapindetailsdialog.h"
#include "../core/logger/consolelogger.h"
#include "../core/logger/filelogger.h"
#include "../core/logger/log.h"
#include "../core/logger/loggermanager.h"
#include "../core/pluginstorage.h"
#include "../core/snapindetailsdialog.h"
#include "../core/snapindetailsfactory.h"
#include "../core/snapinloader.h"
#include "../core/snapinmanager.h"
#include "../core/logger/sysloglogger.h"
#include "../core/translatorstorage.h"
#include "../core/version.h"
#include "../gui/commandlineparser.h"
#include "../gui/mainwindow.h"

#include <QApplication>

int main(int argc, char **argv)
{
    // Register types for factory.
    gpui::SnapInDetailsFactory::define<gpui::SnapInDetailsDialog>("ISnapIn");
    gpui::SnapInDetailsFactory::define<gpui::CompositeSnapInDetailsDialog>("ICompositeSnapIn");

    // Load plugins and snap-ins.
    auto snapInManager = std::make_unique<gpui::SnapInManager>();
    auto snapInLoader  = std::make_unique<gpui::SnapInLoader>(snapInManager.get());

    snapInLoader->loadDefaultSnapIns();

    // Create window.
    QApplication app(argc, argv);

    // NOTE: set app variables which will be used to
    // construct settings path
    app.setOrganizationName(QCoreApplication::translate("main", "BaseALT Ltd."));
    app.setOrganizationDomain("basealt.ru");
    app.setApplicationName("GPUI");
    app.setApplicationVersion(getApplicationVersion());

    QLocale locale;
    QString language = locale.system().name().split("_").at(0);
    TranslatorStorage translatorStorage;

    translatorStorage.loadTranslators(language);
    translatorStorage.loadQtTranslations(language, "qt_");

    gpui::CommandLineParser parser(app);
    gpui::CommandLineOptions options{};
    QString errorMessage{};

    gpui::CommandLineParser::CommandLineParseResult parserResult = parser.parseCommandLine(&options, &errorMessage);

    switch (parserResult)
    {
    case gpui::CommandLineParser::CommandLineError:
        printf("%s \n", qPrintable(errorMessage));
        parser.showHelp();
        return 1;
    case gpui::CommandLineParser::CommandLineHelpRequested:
        parser.showHelp();
        return 0;
    case gpui::CommandLineParser::CommandLineVersionRequested:
        parser.showVersion();
        return 0;
    case gpui::CommandLineParser::CommandLineOk:
    default:
        break;
    }

    auto logManager = gpui::logger::LoggerManager::getInstance();

    auto consoleLogger = std::make_shared<gpui::logger::ConsoleLogger>();
    consoleLogger->setLogLevel(gpui::logger::All);
    logManager->addLogger(consoleLogger);

    auto syslogLogger = std::make_shared<gpui::logger::SyslogLogger>();
    syslogLogger->setLogLevel(gpui::logger::WarningAndAbove);
    logManager->addLogger(syslogLogger);

    auto fileLogger = std::make_shared<gpui::logger::FileLogger>();
    fileLogger->setLogLevel(gpui::logger::InfoAndAbove);
    logManager->addLogger(fileLogger);

    GPUI_DEBUG("debug test");
    GPUI_INFO("info test");
    GPUI_WARNING("warning test");
    GPUI_ERROR("error test");
    GPUI_CRITICAL("critical test");

    gpui::MainWindow window(options, snapInManager.get(), &translatorStorage);
    window.show();

    return app.exec();
}
