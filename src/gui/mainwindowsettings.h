#ifndef MAINWINDOWSETTINGS_H
#define MAINWINDOWSETTINGS_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../core/settings.h"

#include <QObject>

namespace gpui
{

class MainWindowSettingsPrivate;

class MainWindowSettings : public QObject
{
public:
    Q_OBJECT

    Q_PROPERTY(QByteArray geometry MEMBER geometry)
    Q_PROPERTY(QByteArray windowState MEMBER windowState)
    Q_PROPERTY(QByteArray splitterState MEMBER splitterState)
    Q_PROPERTY(QString languageState MEMBER languageState)
    Q_PROPERTY(QString admxPath MEMBER admxPath)

public:
    MainWindowSettings(MainWindow *window, Ui::MainWindow *ui, Settings *setttings);
    ~MainWindowSettings();

    void saveSettings();
    void loadSettings();

public:
    QByteArray geometry{} ;
    QByteArray windowState{};
    QByteArray splitterState{};
    QString languageState{};
    QString admxPath{};

private:
    MainWindowSettingsPrivate *d;

private:
    void convertSettingsToProperties();
    void convertPropertiesToSettings();

    MainWindowSettings(const MainWindowSettings&) = delete;
    MainWindowSettings(MainWindowSettings &&) = delete;
    MainWindowSettings &operator=(const MainWindowSettings &) = delete;
    MainWindowSettings &operator=(MainWindowSettings &&) = delete;
};

} //gpui

#endif // MAINWINDOWSETTINGS_H
