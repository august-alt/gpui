#include "scriptstreemodel.h"

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

public:
    ScriptsFolderItem()
        : ModelView::GroupItem("ScriptsFolderItem")
    {
        addProperty(NODE_ID, QUuid::createUuid());
        addProperty(PARENT_ID, QUuid::createUuid());
        addProperty(NAMESPACE, "");
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
    auto machineItem = insertItem<ScriptsFolderItem>(rootItem());
    machineItem->setDisplayName("Machine");
    auto machineUuid = QUuid("{123e4567-e89b-12d3-a456-426652340000}");
    machineItem->setProperty(ScriptsFolderItem::NODE_ID, machineUuid);

    auto machineSystemSettings = insertItem<ScriptsFolderItem>(machineItem);
    machineSystemSettings->setDisplayName("System settings");
    machineSystemSettings->setProperty(ScriptsFolderItem::PARENT_ID, machineUuid);

    auto machineScripts = insertItem<ScriptsFolderItem>(machineSystemSettings);
    machineScripts->setDisplayName("Scripts");
    machineScripts->setProperty(ScriptsFolderItem::NAMESPACE, "Machine");

    auto userItem = insertItem<ScriptsFolderItem>(rootItem());
    userItem->setDisplayName("User");
    auto userUuid = QUuid("{123e4567-e89b-12d3-a456-426652340001}");
    userItem->setProperty(ScriptsFolderItem::NODE_ID, userUuid);

    auto userSystemSetting = insertItem<ScriptsFolderItem>(userItem);
    userSystemSetting->setDisplayName("System settings");
    userSystemSetting->setProperty(ScriptsFolderItem::PARENT_ID, userUuid);

    auto userScripts = insertItem<ScriptsFolderItem>(userSystemSetting);
    userScripts->setDisplayName("Scripts");
    userScripts->setProperty(ScriptsFolderItem::NAMESPACE, "User");
}
} // namespace scripts_plugin
