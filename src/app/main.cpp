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

#include "../gui/commandlineparser.h"
#include "../gui/mainwindow.h"
#include "../model/pluginstorage.h"

#include <QApplication>

int main(int argc, char ** argv) {
    // Load plugins.
    gpui::PluginStorage::instance()->loadDefaultPlugins();

    // Create window.
    QApplication app(argc, argv);

    gpui::CommandLineParser parser(app);
    gpui::CommandLineOptions options;
    QString errorMessage;

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

    // NOTE: set app variables which will be used to
    // construct settings path
    app.setOrganizationName("BaseALT");
    app.setOrganizationDomain("basealt.ru");
    app.setApplicationName("GPUI");
    
    gpui::MainWindow window(options);
    window.show();

    return app.exec();
}
