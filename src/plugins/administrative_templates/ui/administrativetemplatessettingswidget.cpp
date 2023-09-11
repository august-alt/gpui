#include "administrativetemplatessettingswidget.h"
#include "ui_administrativetemplatessettingswidget.h"

namespace gpui
{
AdministrativeTemplatesSettingsWidget::AdministrativeTemplatesSettingsWidget(ISnapInManagementSettings *manager,
                                                                             QWidget *parent)
    : ISettingsWidget(manager, parent)
    , ui(new Ui::AdministrativeTemplatesSettingsWidget())
    , settings{}
{
    ui->setupUi(this);
}

AdministrativeTemplatesSettingsWidget::~AdministrativeTemplatesSettingsWidget()
{
    delete ui;
}

QString AdministrativeTemplatesSettingsWidget::getName() const
{
    return QStringLiteral("Administrative Templates");
}

void AdministrativeTemplatesSettingsWidget::saveSettings()
{
    settings.enableLayout = ui->enableLayoutCheckBox->isChecked();

    getSettingsManager()->saveSettings(getName(), &settings);
}

void AdministrativeTemplatesSettingsWidget::loadSettings()
{
    getSettingsManager()->loadSettings(getName(), &settings);

    updateWidgetFromSettings();
}

void AdministrativeTemplatesSettingsWidget::updateWidgetFromSettings()
{
    ui->enableLayoutCheckBox->setChecked(settings.enableLayout);
}

} // namespace gpui
