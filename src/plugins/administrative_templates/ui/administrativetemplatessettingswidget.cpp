#include "administrativetemplatessettingswidget.h"
#include "ui_administrativetemplatessettingswidget.h"

namespace gpui
{
class AdministrativeTemplatesSettingsWidgetPrivate
{
public:
    AdministrativeTemplatesSettingsWidgetPrivate() { ui = new Ui::AdministrativeTemplatesSettingsWidget(); }
    ~AdministrativeTemplatesSettingsWidgetPrivate() { delete ui; }

public:
    AdministrativeTemplatesSettingsObject settings{};

    Ui::AdministrativeTemplatesSettingsWidget *ui{nullptr};

private:
    AdministrativeTemplatesSettingsWidgetPrivate(const AdministrativeTemplatesSettingsWidgetPrivate &) = delete;
    AdministrativeTemplatesSettingsWidgetPrivate(AdministrativeTemplatesSettingsWidgetPrivate &&)      = delete;
    AdministrativeTemplatesSettingsWidgetPrivate &operator=(const AdministrativeTemplatesSettingsWidgetPrivate &)
        = delete;
    AdministrativeTemplatesSettingsWidgetPrivate &operator=(AdministrativeTemplatesSettingsWidgetPrivate &&) = delete;
};

AdministrativeTemplatesSettingsWidget::AdministrativeTemplatesSettingsWidget(ISnapInSettingsManager *manager,
                                                                             QWidget *parent)
    : ISettingsWidget(manager, parent)
    , d(new AdministrativeTemplatesSettingsWidgetPrivate())
{
    d->ui->setupUi(this);
}

AdministrativeTemplatesSettingsWidget::~AdministrativeTemplatesSettingsWidget()
{
    delete d;
}

QString AdministrativeTemplatesSettingsWidget::getName() const
{
    return QStringLiteral("Administrative Templates");
}

void AdministrativeTemplatesSettingsWidget::saveSettings()
{
    d->settings.enableLayout = d->ui->enableLayoutCheckBox->isChecked();

    getSettingsManager()->saveSettings(getName(), &d->settings);
}

void AdministrativeTemplatesSettingsWidget::loadSettings()
{
    getSettingsManager()->loadSettings(getName(), &d->settings);

    updateWidgetFromSettings();
}

void AdministrativeTemplatesSettingsWidget::updateWidgetFromSettings()
{
    d->ui->enableLayoutCheckBox->setChecked(d->settings.enableLayout);
}

} // namespace gpui
