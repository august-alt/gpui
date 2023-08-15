/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#include "securitywidget.h"
#include "ui_securitywidget.h"

#include "../administrative_templates/bundle/itemtype.h"
#include "../administrative_templates/bundle/policyroles.h"

#include "../sdmx/securitydefinition.h"
#include "../presentation/securitypresentation.h"

#include "presentationbuilder.h"

using namespace model::bundle;

namespace security
{
typedef std::shared_ptr<::security::SecurityDefinition> SecurityPtr;
typedef std::shared_ptr<::security::SecurityPresentation> PresentationPtr;


SecurityWidget::SecurityWidget(QWidget *parent)
    : PluginWidgetInterface(parent)
    , ui(new Ui::SecurityWidget())
{
    ui->setupUi(this);
}

SecurityWidget::~SecurityWidget()
{
    delete ui;
}

void SecurityWidget::setModelIndex(const QModelIndex &index)
{
    const QAbstractItemModel *model = index.model();

    if (model)
    {
        ui->securityNameLabel->setText((tr("Security policy: ") + model->data(index, Qt::DisplayRole).value<QString>()).trimmed());
        ui->descriptionTextEdit->setText(model->data(index, PolicyRoles::EXPLAIN_TEXT).value<QString>());

        if (model->data(index, PolicyRoles::ITEM_TYPE).value<uint>() == ItemType::ITEM_TYPE_POLICY)
        {
            auto presentation = model->data(index, PolicyRoles::PRESENTATION).value<PresentationPtr>();
            auto security     = model->data(index, PolicyRoles::POLICY).value<SecurityPtr>();

            if (presentation && security)
            {
                auto layout = ::security::PresentationBuilder::build(
                    {
                        *presentation,
                        *security,
                    });

                if (layout)
                {
                    ui->contentScrollArea->widget()->setLayout(layout);
                }
            }
        }
    }
}

void SecurityWidget::setUserSecuritySource()
{
}

void SecurityWidget::setMachineSecuritySource()
{
}

}

Q_DECLARE_METATYPE(std::shared_ptr<::security::SecurityDefinition>)
Q_DECLARE_METATYPE(std::shared_ptr<::security::SecurityPresentation>)
