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

#ifndef GPUI_PREFERENCES_WIDGET_H
#define GPUI_PREFERENCES_WIDGET_H

#include <memory>

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui
{
class PreferencesWidget;
}
QT_END_NAMESPACE

namespace ModelView
{
class SessionModel;
class ViewModel;
class ViewModelDelegate;
} // namespace ModelView

namespace preferences
{
class PreferencesTreeProxyModel;

class PreferencesWidget : public QWidget
{
public:
    Q_OBJECT
public:
    explicit PreferencesWidget(::ModelView::SessionModel *model, QWidget *parent = nullptr);
    ~PreferencesWidget();

    void setModel(::ModelView::SessionModel *model);

private:
    PreferencesWidget(const PreferencesWidget &) = delete;            // copy ctor
    PreferencesWidget(PreferencesWidget &&)      = delete;            // move ctor
    PreferencesWidget &operator=(const PreferencesWidget &) = delete; // copy assignment
    PreferencesWidget &operator=(PreferencesWidget &&) = delete;      // move assignment

private:
    Ui::PreferencesWidget *ui;

private:
    std::unique_ptr<::ModelView::ViewModel> m_verticalViewModel;
    std::unique_ptr<::ModelView::ViewModel> m_horizontalViewModel;
    std::unique_ptr<QItemSelectionModel> m_selectionModel;
    std::unique_ptr<::ModelView::ViewModelDelegate> m_delegate;
    std::unique_ptr<std::map<std::string, std::unique_ptr<::ModelView::SessionModel>>> m_modelsMap;
    std::unique_ptr<PreferencesTreeProxyModel> m_proxyModel;

private:
    void setupConnections();
};

} // namespace preferences

#endif //GPUI_PREFERENCES_WIDGET_H
