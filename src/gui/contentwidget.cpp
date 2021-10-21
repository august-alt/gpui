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

    std::unique_ptr<model::registry::PolicyStateManager> manager;
    model::command::CommandGroup commandGroup;
    bool cancelFlag = false;
    QModelIndex currentIndex;
    ContentWidget::PolicyWidgetState state = ContentWidget::PolicyWidgetState::STATE_NOT_CONFIGURED;
};

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

    QPushButton* applyButton = ui->policyStateButtonBox->button(QDialogButtonBox::Apply);
    connect(applyButton, &QPushButton::clicked, ui->policyStateButtonBox, &QDialogButtonBox::accepted);

    connect(ui->policyStateButtonBox, &QDialogButtonBox::accepted, this, &ContentWidget::onApplyClicked);
    connect(ui->policyStateButtonBox, &QDialogButtonBox::rejected, this, &ContentWidget::onCancelClicked);
}

ContentWidget::~ContentWidget()
{
    delete ui;
    delete d;
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
        ui->contentScrollArea->setDisabled(false);
    }break;
    case STATE_DISABLED:
    case STATE_NOT_CONFIGURED:
    default:
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

    if (d->commandGroup.canExecute() && !d->cancelFlag)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, QObject::tr("Save settings dialog"),
                                      QObject::tr("Policy settings were modified do you want to save them?"),
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            onApplyClicked();
        } else {
            return;
        }
    }

    d->cancelFlag = false;

    if (ui->contentScrollArea->widget()->layout())
    {
        delete ui->contentScrollArea->takeWidget();

        ui->contentScrollArea->setWidget(new QWidget(this));
    }

    if (model)
    {
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
                auto layout = ::gui::PresentationBuilder::build(*presentation, *policy, *source, d->commandGroup);

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
    if (d->state == ContentWidget::STATE_ENABLED)
    {
        d->commandGroup.execute();
        d->commandGroup.clear();
    }

    savePolicyChanges();
}

void ContentWidget::onCancelClicked()
{
    d->cancelFlag = true;
    d->commandGroup.clear();
    onListItemClicked(d->currentIndex);
}


void gpui::ContentWidget::setPolicyWidgetsVisible(bool visible)
{
    ui->supportedOnLabel->setVisible(visible);
    ui->supportedOnTextEdit->setVisible(visible);

    ui->groupBox->setVisible(visible);

    ui->contentScrollArea->setVisible(visible);
    ui->contentListView->setVisible(!visible);
    ui->policyStateButtonBox->setVisible(visible);
    ui->contentWidget->setVisible(visible);
}

}

Q_DECLARE_METATYPE(std::shared_ptr<::model::presentation::Presentation>)
Q_DECLARE_METATYPE(std::shared_ptr<::model::admx::Policy>)
