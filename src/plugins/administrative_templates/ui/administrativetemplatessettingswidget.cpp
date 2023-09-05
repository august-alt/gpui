#include "administrativetemplatessettingswidget.h"
#include "ui_administrativetemplatessettingswidget.h"

namespace gpui
{
AdministrativeTemplatesSettingsWidget::AdministrativeTemplatesSettingsWidget(QWidget *parent)
    : ISettingsWidget(parent)
    , ui(new Ui::AdministrativeTemplatesSettingsWidget())
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
    // TODO: Implement.
}

} // namespace gpui
