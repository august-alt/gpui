
#include "modelbuilder.h"
#include "shortcutroles.h"

#include <QStandardItem>
#include <QStandardItemModel>

#include <QDebug>

namespace gpui {

QStandardItem* itemCreator(const ::xsd::cxx::tree::string< char, ::xml_schema::SimpleType >& shortcutField)
{
    return new QStandardItem(QString::fromStdString(shortcutField));
}

QStandardItem* itemCreator(const unsigned char& shortcutField)
{
    return new QStandardItem(QString::number(shortcutField));
}

template <typename T>
void createItemIfExist(const T& shortcutField, int index, int role, std::unique_ptr<QStandardItemModel>& model)
{
    if (shortcutField.present())
    {
        QStandardItem* categoryItem = itemCreator(shortcutField.get());
        model->setItem(index, role, categoryItem);
    }
}

template <typename T>
void createItem(T& shortcutField, int index, int role, std::unique_ptr<QStandardItemModel>& model)
{
    QStandardItem* categoryItem = itemCreator(shortcutField);
    model->setItem(index, role, categoryItem);
}

template <typename T>
void createComboBoxValue(T& shortcutField, int index, int role, std::unique_ptr<QStandardItemModel>& model,
                         QStringList possibleValue)
{
    unsigned char currentIndex = 0;

    for (unsigned char i = 0; i < possibleValue.size(); ++i)
    {
        if (shortcutField.compare(possibleValue[i].toStdString()) == 0)
        {
            currentIndex = i + 1;
        }
    }

    createItem(currentIndex, index, role, model);
}

std::unique_ptr<QStandardItemModel> ModelBuilder::schemaToModel(std::unique_ptr<Shortcuts> &shortcuts)
{
    std::unique_ptr<QStandardItemModel> model = std::make_unique<QStandardItemModel>(0, ShortcutRoles::ALL_SHORTCUT_ROLES);

    for (size_t index = 0; index < shortcuts->Shortcut().size(); ++index)
    {
        auto shortcut = shortcuts->Shortcut()[index];

        createItem(shortcut.name(), index, CommonRoles::NAME, model);

        auto properties = shortcut.Properties().front();

        if (properties.action().present())
        {
            createComboBoxValue(properties.action().get(), index, ShortcutRoles::ACTION, model, {"R", "U", "D"});
        }

        createItemIfExist(properties.arguments(), index, ShortcutRoles::ARGUMENTS, model);

        createItemIfExist(properties.comment(), index, ShortcutRoles::COMMENT, model);

        createItemIfExist(properties.iconIndex(), index, ShortcutRoles::ICON_INDEX, model);

        createItemIfExist(properties.iconPath(), index, ShortcutRoles::ICON_PATH, model);

        createItemIfExist(properties.pidl(), index, ShortcutRoles::PIDL, model);

        createItemIfExist(properties.startIn(), index, ShortcutRoles::START_IN, model);

        createItemIfExist(properties.shortcutKey(), index, ShortcutRoles::SHORTCUT_KEY, model);

        createItem(properties.shortcutPath(), index, ShortcutRoles::SHORTCUT_PATH, model);

        createItem(properties.targetPath(), index, ShortcutRoles::TARGET_PATH, model);

        createComboBoxValue(properties.targetType(), index, ShortcutRoles::TARGET_TYPE, model, {"URL", "SHELL"});

        createItemIfExist(properties.window(), index, ShortcutRoles::WINDOW, model);
    }

    return model;
}

}
