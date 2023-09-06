#ifndef GPUI_ADMINISTRATIVE_TEMPLATES_SETTINGS_OBJECT_H
#define GPUI_ADMINISTRATIVE_TEMPLATES_SETTINGS_OBJECT_H

#include <QObject>

class AdministrativeTemplatesSettingsObject : public QObject
{
public:
    Q_OBJECT

public:
    Q_PROPERTY(bool enableLayout MEMBER enableLayout)

    bool enableLayout = false;
};

#endif //GPUI_ADMINISTRATIVE_TEMPLATES_SETTINGS_OBJECT_H
