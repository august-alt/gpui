#include "snapinmanagementsettings.h"

#include <QMetaProperty>

gpui::SnapInManagementSettings::SnapInManagementSettings()
    : settings("BaseALT", "GPUISnapIns")
{}

void gpui::SnapInManagementSettings::saveSattings(QObject *snapinSettings)
{
    //where do we count from zero or one?
    for (int i = 1; i < snapinSettings->metaObject()->propertyCount(); ++i)
    {
        QMetaProperty currentProperty = snapinSettings->metaObject()->property(i);

        QString propertyName = currentProperty.name();

        QByteArray charArray = propertyName.toLocal8Bit();

        const char *propName = charArray.data();

        settings.setValue(propName, QVariant(snapinSettings->property(propName)));
    }
}
