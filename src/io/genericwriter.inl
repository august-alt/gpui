#include "genericwriter.h"

#include "../core/pluginstorage.h"

#include <fstream>

#include <QDebug>
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
        qWarning() << "Format supporting: " << pluginName << " not found.";

        return false;
    }

    std::ofstream file;

    file.open(filename, std::ios::out);

    if (file.good())
    {
        if(!format->write(file, fileData))
        {
            qWarning() << "Error while writing file contents: " << format->getErrorString().c_str();

            return false;
        }
    }
    else
    {
        qWarning() << "Unable to open a file: " << filename.c_str();
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
        qWarning() << "Format supporting: " << pluginName << " not found.";

        return false;
    }

    if(fileContent.good())
    {
        if(!format->write(fileContent, fileData))
        {
            qWarning() << "Error while writing file contents: " << format->getErrorString().c_str();

            return false;
        }
    }

    delete format;

    return true;
}
}
