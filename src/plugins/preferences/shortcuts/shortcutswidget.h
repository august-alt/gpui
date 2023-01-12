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

#ifndef GPUI_SHORTCUTS_WIDGET_H
#define GPUI_SHORTCUTS_WIDGET_H

#include <QtWidgets>

#include "common/basepreferencewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class ShortcutsWidget;
}
QT_END_NAMESPACE

class QDataWidgetMapper;

namespace ModelView
{
class ViewModel;
class ViewModelDelegate;
class SessionItem;
} // namespace ModelView

namespace preferences
{
class ShortcutsItem;
class ShortcutsItemController;

//! Folder item representation for editor.

class ShortcutsWidget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit ShortcutsWidget(QWidget *parent = nullptr, ShortcutsItem *item = nullptr);
    ~ShortcutsWidget() override;

    void setItem(ModelView::SessionItem *item) override;

    bool validate() override;

    QString name() const override;

private slots:
    void on_shortkutKeySequenceEdit_editingFinished();

private slots:
    void on_targetTypeComboBox_currentIndexChanged(int index);

private slots:
    void on_actionComboBox_currentIndexChanged(int index);
    void on_iconFilePathLineEdit_textChanged(const QString &text);
    void on_iconFilePathToolButton_clicked();
    void on_locationComboBox_currentIndexChanged(int index);
    void on_nameToolButton_clicked();
    void on_startInToolButton_clicked();
    void on_targetPathToolButton_clicked();

private:
    QString openFileOrFolder(bool folderMode);

    void updateCurrentSequence(const QKeySequence &sequence);
    void setDefaultPlaceholderText();

private:
    ShortcutsWidget(const ShortcutsWidget &) = delete;            // copy ctor
    ShortcutsWidget(ShortcutsWidget &&)      = delete;            // move ctor
    ShortcutsWidget &operator=(const ShortcutsWidget &) = delete; // copy assignment
    ShortcutsWidget &operator=(ShortcutsWidget &&) = delete;      // move assignment

private:
    //!< Underlying item of this view.
    ShortcutsItem *m_item{nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

private:
    Ui::ShortcutsWidget *ui{nullptr};
};

} // namespace preferences

#endif //GPUI_SHORTCUTS_WIDGET_H
