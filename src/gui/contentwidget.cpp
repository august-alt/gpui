/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd. <org@basealt.ru>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#include "contentwidget.h"

#include "ui_contentwidget.h"

#include "presentationbuilder.h"

#include "../model/registry/abstractregistrysource.h"
#include "../model/registry/policystatemanager.h"
#include "../model/admx/policy.h"
#include "../model/admx/policyelement.h"

#include "../model/bundle/policyroles.h"
#include "../model/commands/commandgroup.h"

using namespace ::model::bundle;

namespace gpui {

typedef std::shared_ptr<::model::presentation::Presentation> PresentationPtr;
typedef std::shared_ptr<::model::admx::Policy> PolicyPtr;

class ContentWidgetPrivate
{
public:
    model::registry::AbstractRegistrySource* userSource = nullptr;
    model::registry::AbstractRegistrySource* machineSource = nullptr;

    std::unique_ptr<model::registry::PolicyStateManager> manager = nullptr;
    bool dataChanged = false;
    bool stateEnabled = false;
    QModelIndex currentIndex {};
    ContentWidget::PolicyWidgetState state = ContentWidget::PolicyWidgetState::STATE_NOT_CONFIGURED;
};

void gpui::ContentWidget::connectDialogBoxSignals()
{
    connect(ui->okPushButton, &QPushButton::clicked, this, &ContentWidget::onApplyClicked);
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &ContentWidget::onCancelClicked);
}

ContentWidget::ContentWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContentWidget())
    , d(new ContentWidgetPrivate())
{
    ui->setupUi(this);

    setPolicyWidgetsVisible(false);

    connect(ui->contentListView, &QListView::clicked, this, &ContentWidget::onListItemClicked);
    connect(this, &ContentWidget::modelItemSelected, this, &ContentWidget::onListItemClicked);

    connect(ui->notConfiguredRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            setPolicyWidgetState(STATE_NOT_CONFIGURED);
            qWarning() << "Setting state not configured" << d->manager.get();
            if (d->manager)
            {
                d->manager->setupPolicyState(model::registry::PolicyStateManager::STATE_NOT_CONFIGURED);
            }
        }
    });
    connect(ui->enabledRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            setPolicyWidgetState(STATE_ENABLED);
            qWarning() << "Setting state enabled" << d->manager.get();
            if (d->manager)
            {
                d->manager->setupPolicyState(model::registry::PolicyStateManager::STATE_ENABLED);
            }
        }
    });
    connect(ui->disabledRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            setPolicyWidgetState(STATE_DISABLED);
            qWarning() << "Setting state disabled" << d->manager.get();
            if (d->manager)
            {
                d->manager->setupPolicyState(model::registry::PolicyStateManager::STATE_DISABLED);
            }
        }
    });

    connectDialogBoxSignals();
}

ContentWidget::~ContentWidget()
{
    delete ui;
    delete d;
}

void ContentWidget::setEventFilter(QObject *eventFilter)
{
    ui->contentListView->installEventFilter(eventFilter);
}

void ContentWidget::setModel(QAbstractItemModel* model)
{
    ui->contentListView->setModel(model);
}

void ContentWidget::setSelectionModel(QItemSelectionModel* selectionModel)
{
    ui->contentListView->setSelectionModel(selectionModel);
}

void ContentWidget::setUserRegistrySource(model::registry::AbstractRegistrySource *registrySource)
{
    d->userSource = registrySource;
}

void ContentWidget::setMachineRegistrySource(model::registry::AbstractRegistrySource *registrySource)
{
    d->machineSource = registrySource;
}

void ContentWidget::setPolicyWidgetState(ContentWidget::PolicyWidgetState state)
{
    switch (state) {
    case STATE_ENABLED:
    {
        d->stateEnabled = true;
        ui->contentScrollArea->setDisabled(false);
    }break;
    case STATE_DISABLED:
    case STATE_NOT_CONFIGURED:
    default:
        d->stateEnabled = false;
        ui->contentScrollArea->setDisabled(true);
        break;
    }

    d->state = state;
}

void ContentWidget::onLanguageChaged()
{
    ui->retranslateUi(this);
}

void ContentWidget::onListItemClicked(const QModelIndex &index)
{
    d->currentIndex = index;

    const QAbstractItemModel* model = index.model();

    if (d->dataChanged)
    {
        QMessageBox messageBox(QMessageBox::Question,
                    QObject::tr("Save settings dialog"),
                    QObject::tr("Policy settings were modified do you want to save them?"),
                    QMessageBox::Yes | QMessageBox::No,
                    this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Yes"));
        messageBox.setButtonText(QMessageBox::No, tr("No"));
        int reply = messageBox.exec();

        switch (reply)
        {
        case QMessageBox::Yes:
            emit ui->okPushButton->clicked();
            onApplyClicked();
            break;
        case QMessageBox::No:
            onCancelClicked();
            break;
        default:
            break;
        }
    }

    d->dataChanged = false;

    if (ui->contentScrollArea->widget()->layout())
    {
        delete ui->contentScrollArea->takeWidget();

        ui->contentScrollArea->setWidget(new QWidget(this));
    }

    if (model)
    {
        ui->policyNameLabel->setText((tr("Policy: ") + model->data(index, Qt::DisplayRole).value<QString>()).trimmed());
        ui->descriptionTextEdit->setText(model->data(index, PolicyRoles::EXPLAIN_TEXT).value<QString>());

        d->manager = nullptr;

        if (model->data(index, PolicyRoles::ITEM_TYPE).value<uint>() == 1)
        {
            setPolicyWidgetsVisible(true);
            setPolicyWidgetState(STATE_NOT_CONFIGURED);

            ui->notConfiguredRadioButton->setChecked(true);
            ui->supportedOnTextEdit->setText(model->data(index, PolicyRoles::SUPPORTED_ON).value<QString>());

            auto presentation = model->data(index, PolicyRoles::PRESENTATION).value<PresentationPtr>();
            auto policy = model->data(index, PolicyRoles::POLICY).value<PolicyPtr>();
            model::registry::AbstractRegistrySource* source = d->userSource;

            if (policy && d->machineSource)
            {
                if (policy->policyType == model::admx::PolicyType::Machine)
                {
                    source = d->machineSource;
                }
            }

            if (source && policy)
            {
                d->manager = std::make_unique<model::registry::PolicyStateManager>(*source, *policy);

                auto state = d->manager->determinePolicyState();
                if (state == model::registry::PolicyStateManager::STATE_ENABLED)
                {
                    ui->enabledRadioButton->setChecked(true);
                }

                if (state == model::registry::PolicyStateManager::STATE_DISABLED)
                {
                    ui->disabledRadioButton->setChecked(true);
                }

                if (state == model::registry::PolicyStateManager::STATE_NOT_CONFIGURED)
                {
                    ui->notConfiguredRadioButton->setChecked(true);
                }
            }

            if (presentation && policy)
            {
                ui->okPushButton->disconnect();
                ui->cancelPushButton->disconnect();
                auto layout = ::gui::PresentationBuilder::build({
                                                                *presentation, *policy, *source,
                                                                *ui->okPushButton, d->dataChanged,
                                                                d->stateEnabled
                                                                });
                connectDialogBoxSignals();

                ui->contentScrollArea->widget()->setLayout(layout);                
            }
        }
        else
        {
            setPolicyWidgetsVisible(false);

            ui->contentListView->setRootIndex(index);
        }
    }
}

void ContentWidget::onApplyClicked()
{
    d->dataChanged = false;
    savePolicyChanges();
}

void ContentWidget::onCancelClicked()
{
    d->dataChanged = false;
    onListItemClicked(d->currentIndex);
}


void gpui::ContentWidget::setPolicyWidgetsVisible(bool visible)
{
    ui->supportedOnLabel->setVisible(visible);
    ui->supportedOnTextEdit->setVisible(visible);

    ui->groupBox->setVisible(visible);

    ui->contentScrollArea->setVisible(visible);
    ui->contentListView->setVisible(!visible);
    ui->policyStateContainerWidget->setVisible(visible);
    ui->contentWidget->setVisible(visible);

    ui->policyNameFrame->setVisible(visible);
}

}

Q_DECLARE_METATYPE(std::shared_ptr<::model::presentation::Presentation>)
Q_DECLARE_METATYPE(std::shared_ptr<::model::admx::Policy>)
