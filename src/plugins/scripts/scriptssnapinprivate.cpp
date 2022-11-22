#include "scriptssnapinprivate.h"

#include <QDebug>

namespace scripts_plugin
{
ScriptsSnapInPrivate::ScriptsSnapInPrivate(ScriptsSnapIn *scriptsSnapIn)
    : snapIn(scriptsSnapIn)
{}

void ScriptsSnapInPrivate::saveData()
{
    qWarning() << "Scripts snapin saveData()";
    snapIn->onDataSave();
}

void ScriptsSnapInPrivate::reloadData()
{
    qWarning() << "Scripts snapin reloaData()";
    auto path = policyPath.get();
    snapIn->onDataLoad(*path, currentLocale);
}

} // namespace scripts_plugin
