#include "scriptstreemodel.h"
#include "scriptitem.h"

#include <QUuid>

#include <mvvm/model/groupitem.h>
#include <mvvm/model/propertyitem.h>
#include <mvvm/model/taginfo.h>

namespace
{
class ScriptsFolderItem : public ModelView::GroupItem
{
public:
    static inline const std::string NODE_ID   = "NODE_ID";
    static inline const std::string PARENT_ID = "PARENT_ID";
    static inline const std::string NAMESPACE = "NAMESPACE";
    static inline const std::string HELP_MSG  = "HELP_MSG";

public:
    ScriptsFolderItem()
        : ModelView::GroupItem("ScriptsFolderItem")
    {
        addProperty(NODE_ID, QUuid::createUuid());
        addProperty(PARENT_ID, QUuid::createUuid());
        addProperty(NAMESPACE, "");
        addProperty(HELP_MSG, "");
    }

    template<typename T>
    T *addProperty(const std::string &name)
    {
        registerTag(ModelView::TagInfo::propertyTag(name, T().modelType()));
        auto result = insertItem<T>({name, 0});
        result->setDisplayName(name);
        return result;
    }

    inline ModelView::PropertyItem *addProperty(const std::string &name, const char *value)
    {
        return addProperty(name, std::string(value));
    }

    template<typename V>
    ModelView::PropertyItem *addProperty(const std::string &name, const V &value)
    {
        auto property = addProperty<ModelView::PropertyItem>(name);
        property->setData(value);
        if constexpr (std::is_floating_point_v<V>)
            property->setData(ModelView::RealLimits::limitless(), ModelView::ItemDataRole::LIMITS);
        return property;
    }
};

} // namespace

namespace scripts_plugin
{
ScriptsTreeModel::ScriptsTreeModel()
    : ModelView::SessionModel("ScriptsTreeModel")
{
    registerItem<ScriptsFolderItem>();
    populateModel();
}

void ScriptsTreeModel::populateModel()
{
    auto topItem = insertItem<ScriptsFolderItem>(this->rootItem());
    topItem->setDisplayName(QObject::tr("[Local Group Policy]").toStdString());
    topItem->setProperty(ScriptsFolderItem::HELP_MSG, "");
    auto topUuid = QUuid("{123e4567-e89b-12d3-a456-426652340003}");
    topItem->setProperty(ScriptsFolderItem::NODE_ID, topUuid);
    topItem->setProperty(ScriptsFolderItem::HELP_MSG, QObject::tr("Group policy").toStdString());

    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    auto machineNamespace = insertItem<ScriptsFolderItem>(topItem);
    machineNamespace->setDisplayName(QObject::tr("Machine").toStdString());
    auto machineUuid = QUuid("{123e4567-e89b-12d3-a456-426652340000}");
    machineNamespace->setProperty(ScriptsFolderItem::NODE_ID, machineUuid);
    machineNamespace->setProperty(ScriptsFolderItem::PARENT_ID, topUuid);
    machineNamespace->setProperty(ScriptsFolderItem::HELP_MSG,
                                  QObject::tr("Machine level policies").toStdString());

    auto machineSystemSettings = insertItem<ScriptsFolderItem>(machineNamespace);
    machineSystemSettings->setDisplayName(QObject::tr("System settings").toStdString());
    machineSystemSettings->setProperty(ScriptsFolderItem::PARENT_ID, machineUuid);
    machineSystemSettings->setProperty(ScriptsFolderItem::HELP_MSG,
                                       QObject::tr("System settings for computer").toStdString());

    auto machineScripts = insertItem<ScriptsFolderItem>(machineSystemSettings);
    machineScripts->setDisplayName(QObject::tr("Scripts").toStdString());
    machineScripts->setProperty(ScriptsFolderItem::NAMESPACE, "Machine");
    machineScripts->setProperty(ScriptsFolderItem::HELP_MSG,
                                QObject::tr("Scripts for computer").toStdString());

    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    auto userNamespace = insertItem<ScriptsFolderItem>(topItem);
    userNamespace->setDisplayName(QObject::tr("User").toStdString());
    auto userUuid = QUuid("{123e4567-e89b-12d3-a456-426652340001}");
    userNamespace->setProperty(ScriptsFolderItem::NODE_ID, userUuid);
    userNamespace->setProperty(ScriptsFolderItem::PARENT_ID, topUuid);
    userNamespace->setProperty(ScriptsFolderItem::HELP_MSG,
                               QObject::tr("User level policies.").toStdString());

    auto userSystemSetting = insertItem<ScriptsFolderItem>(userNamespace);
    userSystemSetting->setDisplayName(QObject::tr("System settings").toStdString());
    userSystemSetting->setProperty(ScriptsFolderItem::PARENT_ID, userUuid);
    userSystemSetting->setProperty(ScriptsFolderItem::HELP_MSG,
                                   QObject::tr("System settings for user").toStdString());

    auto userScripts = insertItem<ScriptsFolderItem>(userSystemSetting);
    userScripts->setDisplayName(QObject::tr("Scripts").toStdString());
    userScripts->setProperty(ScriptsFolderItem::NAMESPACE, "User");
    userScripts->setProperty(ScriptsFolderItem::HELP_MSG,
                             QObject::tr("Scripts for user").toStdString());
}
} // namespace scripts_plugin
