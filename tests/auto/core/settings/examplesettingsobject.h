#pragma once

#include <QObject>

class ExampleSettingsObject : public QObject
{
public:
    Q_OBJECT
public:
    Q_PROPERTY(QString stringProperty MEMBER stringProperty)
    Q_PROPERTY(int integerProperty MEMBER integerProperty)
    Q_PROPERTY(float floatProperty MEMBER floatProperty)

    QString stringProperty{"string"};
    int integerProperty = 123;
    float floatProperty = 321.0;
};
