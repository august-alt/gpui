#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../../src/core/settings.h"
#include "examplesettingsobject.h"

#include <QApplication>
#include <QObject>
#include <QTest>

bool isPropertiesEqual(QObject *first, QObject *second)
{
    if (first->metaObject()->propertyCount() != second->metaObject()->propertyCount())
    {
        return false;
    }

    for (int i = first->metaObject()->propertyOffset(); i < first->metaObject()->propertyCount(); ++i)
    {
        QMetaProperty firstCurrentProperty = first->metaObject()->property(i);

        int index = second->metaObject()->indexOfProperty(firstCurrentProperty.name());

        if (index == -1)
        {
            return false;
        }

        QMetaProperty secondCurrentProperty = second->metaObject()->property(index);

        if (firstCurrentProperty.read(first) != secondCurrentProperty.read(second))
        {
            return false;
        }
    }

    return true;
}

TEST(Settings, saveLoadSettings)
{
    ExampleSettingsObject firstObj;

    ExampleSettingsObject secondObj;
    secondObj.stringProperty  = "sdfsdfsd";
    secondObj.integerProperty = 1222;
    secondObj.floatProperty   = 345345.0;

    gpui::Settings settings("BaseALT", "GPUITESTS");
    settings.saveSettings("Example", &firstObj);

    settings.loadSettings("Example", &secondObj);

    ASSERT_TRUE(isPropertiesEqual(&firstObj, &secondObj));
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
