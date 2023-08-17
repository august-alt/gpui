#include "genericwriter.h"

#include "../core/pluginstorage.h"

#include <fstream>

#include "../core/logger/log.h"
#include <QString>

namespace io {

template<typename TData, typename TFormat>
bool GenericWriter::save(const std::string &filename, TData *fileData)
{
    QString pluginName = QString::fromStdString(filename);
    pluginName = pluginName.mid(pluginName.lastIndexOf('.') + 1);

    TFormat *format = gpui::PluginStorage::instance()->createPluginClass<TFormat>(pluginName);

    if(!format)
    {
        GPUI_WARNING_STREAM << "Format supporting: " << pluginName << " not found.";

        return false;
    }

    std::ofstream file;

    file.open(filename, std::ios::out);

    if (file.good())
    {
        if(!format->write(file, fileData))
        {
            GPUI_WARNING_STREAM << "Error while writing file contents: " << format->getErrorString().c_str();

            return false;
        }
    }
    else
    {
        GPUI_WARNING_STREAM << "Unable to open a file: " << filename.c_str();
    }

    file.close();

    delete format;

    return true;

}

template<typename TData, typename TFormat>
bool GenericWriter::save(std::ostream &fileContent, const std::string &plName, TData *fileData)
{
    QString pluginName = QString::fromStdString(plName);
    pluginName = pluginName.mid(pluginName.lastIndexOf('.') + 1);

    TFormat *format = gpui::PluginStorage::instance()->createPluginClass<TFormat>(pluginName);

    if(!format)
    {
        GPUI_WARNING_STREAM << "Format supporting: " << pluginName << " not found.";

        return false;
    }

    if(fileContent.good())
    {
        if(!format->write(fileContent, fileData))
        {
            GPUI_WARNING_STREAM << "Error while writing file contents: " << format->getErrorString().c_str();

            return false;
        }
    }

    delete format;

    return true;
}
}
