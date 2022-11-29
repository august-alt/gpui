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

#ifndef SCRIPTSDIALOG_H
#define SCRIPTSDIALOG_H

#include "scriptsmodel.h"

#include "scriptswidget.h"

#include "scriptspowershellwidget.h"

#include "../../../src/core/common.h"

#include <QDataWidgetMapper>
#include <QDialog>
#include <QDialogButtonBox>
#include <qabstractbutton.h>

#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

namespace Ui
{
class ScriptsDialog;
}

namespace scripts_plugin
{
class ScriptItemContainer;

class GPUI_SYMBOL_EXPORT ScriptsDialog : public QDialog
{
public:
    Q_OBJECT

public:
    ScriptsDialog(QWidget *parent = nullptr);
    ~ScriptsDialog();

    void setModels(ScriptsModel *scriptsModel, ScriptsModel *powerScriptsModel, bool isOnStartUp);

private:
    template<typename TWidget>
    void setItem(ModelView::SessionItem *scriptsItem, TWidget &widget);

    ScriptItemContainer *findItemContainer(ScriptsModel *model, std::string section);

public slots:
    void submit();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_Apply();

signals:
    void saveDataSignal();
    void reloaddataSignal();

private:
    ScriptsDialog(const ScriptsDialog &) = delete;
    ScriptsDialog(ScriptsDialog &&)      = delete;
    ScriptsDialog operator=(const ScriptsDialog &) = delete;
    ScriptsDialog operator=(ScriptsDialog &&) = delete;

private:
    Ui::ScriptsDialog *ui;

    bool isStartUpScripts;
};

} // namespace scripts_plugin

#endif // SCRIPTSDIALOG_H
