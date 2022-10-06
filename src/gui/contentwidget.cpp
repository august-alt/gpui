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

#include "../plugins/administrative_templates/bundle/itemtype.h"
#include "../plugins/administrative_templates/bundle/policyroles.h"

using namespace ::model::bundle;

namespace gpui
{
typedef std::shared_ptr<::model::presentation::Presentation> PresentationPtr;
typedef std::shared_ptr<::model::admx::Policy> PolicyPtr;

class ContentWidgetPrivate
{
public:
    QModelIndex currentIndex{};
};

ContentWidget::ContentWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContentWidget())
    , d(new ContentWidgetPrivate())
{
    ui->setupUi(this);

    connect(ui->contentListView, &QListView::clicked, this, &ContentWidget::onListItemClicked);
    connect(this, &ContentWidget::modelItemSelected, this, &ContentWidget::onListItemClicked);
}

ContentWidget::~ContentWidget()
{
    delete ui;
    delete d;
}

void ContentWidget::onLanguageChanged()
{
    ui->retranslateUi(this);
}

void ContentWidget::setEventFilter(QObject *eventFilter)
{
    ui->contentListView->installEventFilter(eventFilter);
}

void ContentWidget::setModel(QAbstractItemModel *model)
{
    ui->contentListView->setModel(model);
}

void ContentWidget::setSelectionModel(QItemSelectionModel *selectionModel)
{
    if (ui->contentListView->selectionModel())
    {
        disconnect(ui->contentListView->selectionModel());
    }

    ui->contentListView->setSelectionModel(selectionModel);
    connect(ui->contentListView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            [&](const QItemSelection &selected, const QItemSelection &deselected) {
                Q_UNUSED(deselected);
                if (selected.isEmpty() || selected.first().indexes().isEmpty())
                {
                    return;
                }

                auto modelIndex = selected.first().indexes().first();

                ui->descriptionTextEdit->setText(modelIndex.data(PolicyRoles::EXPLAIN_TEXT).toString());
                if (modelIndex.data(PolicyRoles::ITEM_TYPE).value<uint>() != ItemType::ITEM_TYPE_POLICY)
                {
                    ui->contentListView->setRootIndex(modelIndex);
                }
                else
                {
                    if (modelIndex.parent().isValid())
                    {
                        ui->contentListView->setRootIndex(modelIndex.parent());
                    }
                }

                modelItemSelected(modelIndex);
            });
}

void ContentWidget::onListItemClicked(const QModelIndex &index)
{
    const QAbstractItemModel *model = index.model();

    if (model)
    {
        ui->descriptionTextEdit->setText(model->data(index, PolicyRoles::EXPLAIN_TEXT).value<QString>());

        if (model->data(index, PolicyRoles::ITEM_TYPE).value<uint>() == ItemType::ITEM_TYPE_POLICY)
        {
            // TODO: Implement widget display.
        }
        else
        {
            ui->contentListView->setRootIndex(index);
        }
    }
}

} // namespace gpui

Q_DECLARE_METATYPE(std::shared_ptr<::model::presentation::Presentation>)
Q_DECLARE_METATYPE(std::shared_ptr<::model::admx::Policy>)
