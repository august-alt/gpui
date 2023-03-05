#ifndef SCRIPTSMODELIO_H
#define SCRIPTSMODELIO_H

#include <scriptsmodel.h>
#include <string>

namespace scripts_plugin
{
class ScriptsModelIo
{
public:
    ScriptsModelIo();

    void loadPolicies(std::string *path,
                      ScriptsModel *userScripts,
                      ScriptsModel *userPowerScripts,
                      ScriptsModel *machineScripts,
                      ScriptsModel *machinePowerScripts);

    void savePolicies(std::string *path,
                      ScriptsModel *userScripts,
                      ScriptsModel *userPowerScripts,
                      ScriptsModel *machineScripts,
                      ScriptsModel *machinePowerScripts);

private:
    void loadIniFile(std::string &path, ScriptsModel *model);

    void saveIniFile(std::string &path, ScriptsModel *model);
};
} // namespace scripts_plugin

#endif // SCRIPTSMODELIO_H
