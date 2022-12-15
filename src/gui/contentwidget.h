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

#ifndef GPUI_CONTENT_WIDGET_H
#define GPUI_CONTENT_WIDGET_H

#include "gui.h"

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui
{
class ContentWidget;
}
QT_END_NAMESPACE

namespace model
{
namespace registry
{
class AbstractRegistrySource;
}
} // namespace model

namespace gui
{
class PluginWidgetInterface;
}

namespace gpui
{
class ContentWidgetPrivate;

class GPUI_GUI_EXPORT ContentWidget : public QWidget
{
    Q_OBJECT
public:
    enum PolicyWidgetState
    {
        STATE_NOT_CONFIGURED = 0,
        STATE_ENABLED        = 1,
        STATE_DISABLED       = 2
    };

public:
    // construction and destruction
    ContentWidget(QWidget *parent = 0);
    ~ContentWidget();

    void setEventFilter(QObject *eventFilter);

    void setModel(QAbstractItemModel *model);
    void setSelectionModel(QItemSelectionModel *selectionModel);

    void onLanguageChanged();

signals:
    void modelItemSelected(const QModelIndex &index);

private:
    Ui::ContentWidget *ui;
    ContentWidgetPrivate *d;

private slots:
    void onListItemClicked(const QModelIndex &index);

private:
    void setWidgetSignals(gui::PluginWidgetInterface *pluginWidget);

private:
    ContentWidget(const ContentWidget &) = delete;            // copy ctor
    ContentWidget(ContentWidget &&)      = delete;            // move ctor
    ContentWidget &operator=(const ContentWidget &) = delete; // copy assignment
    ContentWidget &operator=(ContentWidget &&) = delete;      // move assignment
};
} // namespace gpui

#endif // GPUI_CONTENT_WIDGET_H
