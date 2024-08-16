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

#include "scriptsdialog.h"
#include "groupscriptcontaineritem.h"
#include "scriptitemcontainer.h"
#include "ui_scriptsdialog.h"
#include "ui_scriptspowershellwidget.h"
#include "ui_scriptswidget.h"

#include <mvvm/factories/viewmodelfactory.h>

#include <QDebug>

namespace scripts_plugin
{
ScriptsDialog::ScriptsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ScriptsDialog())
    , isStartUpScripts(false)
    , isUserScripts(false)
{
    ui->setupUi(this);

    QPushButton *applyButton = ui->buttonBox->button(QDialogButtonBox::StandardButton::Apply);

    QObject::connect(applyButton, &QPushButton::clicked, this, &ScriptsDialog::on_Apply);
}

ScriptsDialog::~ScriptsDialog()
{
    delete ui;
}

void ScriptsDialog::setModels(ScriptsModel *scriptsModel, ScriptsModel *powerScriptsModel, bool isOnStartUp, bool isUser)
{
    ScriptItemContainer *scriptsItem      = nullptr;
    ScriptItemContainer *powerScriptsItem = nullptr;

    isStartUpScripts = isOnStartUp;
    isUserScripts    = isUser;

    if (isOnStartUp)
    {
        if (!isUser)
        {
            scriptsItem      = findItemContainer(scriptsModel, "Startup");
            powerScriptsItem = findItemContainer(powerScriptsModel, "Startup");
        }
        else
        {
            scriptsItem      = findItemContainer(scriptsModel, "Logon");
            powerScriptsItem = findItemContainer(powerScriptsModel, "Logon");
        }
    }
    else
    {
        if (!isUser)
        {
            scriptsItem      = findItemContainer(scriptsModel, "Shutdown");
            powerScriptsItem = findItemContainer(powerScriptsModel, "Shutdown");
        }
        else
        {
            scriptsItem      = findItemContainer(scriptsModel, "Logoff");
            powerScriptsItem = findItemContainer(powerScriptsModel, "Logoff");
        }
    }

    if (scriptsItem != nullptr)
    {
        setItem(scriptsItem, ui->scriptsTab);
    }
    if (powerScriptsItem != nullptr)
    {
        setItem(powerScriptsItem, ui->powerShellScriptsTab);
    }
}

void ScriptsDialog::setDescription(const QString &policyName)
{
    QString scriptName;

    if (isUserScripts) 
    {
        scriptName = std::move(isStartUpScripts ? tr("Logon") : tr("Logoff"));
    }
    else 
    {
        scriptName = std::move(isStartUpScripts ? tr("Startup") : tr("Shutdown"));
    }

    QString logonLabelText = tr("Script") + ": \"" + scriptName + "\" " + tr("for")
        + (policyName.isEmpty() ? " " + tr("Default Domain Policy") : " \"" + policyName + "\"");

    QLabel* logonLabel = ui->scriptsTab->findChild<QLabel *>("logonLabel");
    if (logonLabel)
    {
        logonLabel->setText(logonLabelText);
    }
    logonLabel = ui->powerShellScriptsTab->findChild<QLabel *>("logonLabel");
    if (logonLabel)
    {
        logonLabel->setText(logonLabelText);
    }
}

ScriptItemContainer *ScriptsDialog::findItemContainer(ScriptsModel *model, std::string section)
{
    auto containers = model->topItems();
    for (size_t i = 0; i < containers.size(); i++)
    {
        auto item = dynamic_cast<ScriptItemContainer *>(containers[i]);

        if (item)
        {
            auto containerSectionName = item->property<std::string>(ScriptItemContainer::SECTION_NAME);

            if (containerSectionName.compare(section) == 0)
            {
                return item;
            }
        }
    }

    qWarning() << "Section: " << section.c_str() << " not found!";

    return nullptr;
}

template<typename TWidget>
void ScriptsDialog::setItem(ModelView::SessionItem *scriptsItem, TWidget &widget)
{
    auto container = dynamic_cast<ScriptItemContainer *>(scriptsItem);

    if (container)
    {
        widget->setItem(container, isStartUpScripts, isUserScripts);
    }
}

void ScriptsDialog::submit() {}

void ScriptsDialog::on_buttonBox_accepted()
{
    emit(saveDataSignal());
}

void ScriptsDialog::on_buttonBox_rejected()
{
    emit(reloaddataSignal());
}

void ScriptsDialog::on_Apply()
{
    emit(saveDataSignal());
}

} // namespace scripts_plugin
