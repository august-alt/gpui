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

    void loadPolicies(const std::string &path,
                      ScriptsModel *userScripts,
                      ScriptsModel *userPowerScripts,
                      ScriptsModel *machineScripts,
                      ScriptsModel *machinePowerScripts);

    void savePolicies(const std::string &path,
                      ScriptsModel *userScripts,
                      ScriptsModel *userPowerScripts,
                      ScriptsModel *machineScripts,
                      ScriptsModel *machinePowerScripts);

private:
    void loadIniFile(std::string &path, ScriptsModel *model, bool machineScript);

    void saveIniFile(std::string &path, ScriptsModel *model);

    std::string correctPath(const std::string &path);

    void createDirectory(const std::string &directoryName);

    void createDirectories(std::string newPath);

};
} // namespace scripts_plugin

#endif // SCRIPTSMODELIO_H
