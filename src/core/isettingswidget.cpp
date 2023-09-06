#include "isettingswidget.h"

namespace gpui
{
class ISettingsWidgetPrivate
{
public:
    ISnapInManagementSettings *manager = nullptr;
};

ISettingsWidget::ISettingsWidget(ISnapInManagementSettings *manager, QWidget *parent)
    : d(new ISettingsWidgetPrivate)
{
    d->manager = manager;
}

ISnapInManagementSettings *ISettingsWidget::getSettingsManager()
{
    return d->manager;
}

ISettingsWidget::~ISettingsWidget()
{
    delete d;
}

} // namespace gpui
