#include "mainwindowsettings.h"

namespace gpui
{
const QString MAIN_WINDOW_SECTION = "mainwindow";

class MainWindowSettingsPrivate
{
public:
    MainWindowSettingsPrivate(MainWindow *win, Ui::MainWindow *winUi, Settings *set)
    {
        window   = win;
        ui       = winUi;
        settings = set;
    }
    ~MainWindowSettingsPrivate() = default;

public:
    MainWindow *window = nullptr;
    Ui::MainWindow *ui = nullptr;
    Settings *settings = nullptr;

private:
    MainWindowSettingsPrivate(const MainWindowSettingsPrivate &) = delete;
    MainWindowSettingsPrivate(MainWindowSettingsPrivate &&)      = delete;
    MainWindowSettingsPrivate &operator=(const MainWindowSettingsPrivate &) = delete;
    MainWindowSettingsPrivate &operator=(MainWindowSettingsPrivate &&) = delete;
};

MainWindowSettings::MainWindowSettings(MainWindow *window, Ui::MainWindow *ui, Settings *setttings)
    : d(new MainWindowSettingsPrivate(window, ui, setttings))
{}

MainWindowSettings::~MainWindowSettings()
{
    delete d;
}

void MainWindowSettings::saveSettings()
{
    convertSettingsToProperties();
    d->settings->saveSettings(MAIN_WINDOW_SECTION, this);
}

void MainWindowSettings::loadSettings()
{
    d->settings->loadSettings(MAIN_WINDOW_SECTION, this);
    convertPropertiesToSettings();
}

void MainWindowSettings::convertSettingsToProperties()
{
    geometry = d->window->saveGeometry();

    windowState = d->window->saveState();

    splitterState = d->ui->splitter->saveState();

    languageState = d->window->getLanguage();

    admxPath = d->window->getAdmxPath();
}

void MainWindowSettings::convertPropertiesToSettings()
{
    d->window->restoreGeometry(geometry);

    d->window->restoreState(windowState);

    d->ui->splitter->restoreState(splitterState);

    d->window->setLanguage(languageState);

    if (d->window->getAdmxPath().isEmpty())
    {
        d->window->setAdmxPath(admxPath);
    }
}

} // namespace gpui
