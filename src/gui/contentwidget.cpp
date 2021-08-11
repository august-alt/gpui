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

namespace gpui {

typedef std::shared_ptr<::model::presentation::Presentation> PresentationPtr;
typedef std::shared_ptr<::model::admx::Policy> PolicyPtr;

model::registry::AbstractRegistrySource* source = nullptr;

ContentWidget::ContentWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContentWidget())
{
    ui->setupUi(this);

    setPolicyWidgetsVisible(false);

    connect(ui->contentListView, &QListView::clicked, this, &ContentWidget::onListItemClicked);
    connect(this, &ContentWidget::modelItemSelected, this, &ContentWidget::onListItemClicked);

    connect(ui->notConfiguredRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            setPolicyWidgetState(STATE_NOT_CONFIGURED);
        }
    });
    connect(ui->enabledRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            setPolicyWidgetState(STATE_ENABLED);
        }
    });
    connect(ui->disabledRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            setPolicyWidgetState(STATE_DISABLED);
        }
    });
}

ContentWidget::~ContentWidget()
{
    delete ui;
}

void ContentWidget::setModel(QStandardItemModel* model)
{
    ui->contentListView->setModel(model);
}

void ContentWidget::setSelectionModel(QItemSelectionModel* selectionModel)
{
    ui->contentListView->setSelectionModel(selectionModel);
}

void ContentWidget::setRegistrySource(model::registry::AbstractRegistrySource *registrySource)
{
    source = registrySource;
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
}

void ContentWidget::onListItemClicked(const QModelIndex &index)
{
    const QStandardItemModel* model = dynamic_cast<const QStandardItemModel*>(index.model());

    if (ui->contentScrollArea->widget()->layout())
    {
        delete ui->contentScrollArea->takeWidget();

        ui->contentScrollArea->setWidget(new QWidget(this));
    }

    if (model)
    {
        auto item = model->itemFromIndex(index);

        ui->descriptionTextEdit->setText(item->data(Qt::UserRole + 2).value<QString>());

        if (item->data(Qt::UserRole + 1).value<uint>() == 1)
        {
            setPolicyWidgetsVisible(true);
            setPolicyWidgetState(STATE_NOT_CONFIGURED);

            ui->notConfiguredRadioButton->setChecked(true);
            ui->supportedOnTextEdit->setText(item->data(Qt::UserRole + 4).value<QString>());

            auto presentation = item->data(Qt::UserRole +5).value<PresentationPtr>();
            auto policy = item->data(Qt::UserRole + 6).value<PolicyPtr>();

            if (presentation && policy)
            {
                if (source)
                {
                    auto manager = model::registry::PolicyStateManager(*source, *policy);
                    auto state = manager.determinePolicyState();
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

                auto layout = ::gui::PresentationBuilder::build(*presentation, *policy, *source);

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
