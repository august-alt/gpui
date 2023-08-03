#include "scriptssnapinprivate.h"
#include "scriptitemcontainer.h"

#include <QDebug>

namespace scripts_plugin
{
ScriptsSnapInPrivate::ScriptsSnapInPrivate(ScriptsSnapIn *scriptsSnapIn)
    : snapIn(scriptsSnapIn)
{
    //----user scripts model
    auto userLogonItemContainer = userScriptsModel.get()->insertItem<ScriptItemContainer>();
    userLogonItemContainer->setProperty(ScriptItemContainer::SECTION_NAME, "Logon");

    auto userLogoffItemContainer = userScriptsModel.get()->insertItem<ScriptItemContainer>();
    userLogoffItemContainer->setProperty(ScriptItemContainer::SECTION_NAME, "Logoff");

    auto userStartUpItemContainer = userPowerScriptsModel.get()->insertItem<ScriptItemContainer>();
    userStartUpItemContainer->setProperty(ScriptItemContainer::SECTION_NAME, "Logon");

    auto userShutdownItemContainer = userPowerScriptsModel.get()->insertItem<ScriptItemContainer>();
    userShutdownItemContainer->setProperty(ScriptItemContainer::SECTION_NAME, "Logoff");

    //--------------------------------------------------------------------------------------
    //----machine scripts models
    auto machineLogonItemContainer = machineScriptsModel.get()->insertItem<ScriptItemContainer>();
    machineLogonItemContainer->setProperty(ScriptItemContainer::SECTION_NAME, "Startup");

    auto machineLogoffItemContainer = machineScriptsModel.get()->insertItem<ScriptItemContainer>();
    machineLogoffItemContainer->setProperty(ScriptItemContainer::SECTION_NAME, "Shutdown");

    auto machineStartUpItemContainer = machinePowerScriptsModel.get()->insertItem<ScriptItemContainer>();
    machineStartUpItemContainer->setProperty(ScriptItemContainer::SECTION_NAME, "Startup");

    auto machineShutdownItemContainer = machinePowerScriptsModel.get()->insertItem<ScriptItemContainer>();
    machineShutdownItemContainer->setProperty(ScriptItemContainer::SECTION_NAME, "Shutdown");
}

void ScriptsSnapInPrivate::saveData()
{
    snapIn->onDataSave();
}

void ScriptsSnapInPrivate::reloadData()
{
    snapIn->onDataLoad(policyPath, localeName);
}

void ScriptsSnapInPrivate::retranslateModels(std::unique_ptr<ScriptsModel> &models)
{
    if (models)
    {
        for (auto container : models.get()->topItems())
        {
            auto containerItem = dynamic_cast<ScriptItemContainer *>(container);

            if (containerItem)
            {
                containerItem->retranslateStrings();
            }
        }
    }
}

} // namespace scripts_plugin
