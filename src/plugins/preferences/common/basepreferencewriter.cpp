/***********************************************************************************************************************
**
** Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>
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

#include "basepreferencewriter.h"

#include "../../../src/plugins/storage/smb/smbfile.h"

#include <fstream>
#include <sstream>
#include <QDebug>
#include <QMessageBox>

std::string preferences::BasePreferenceWriter::getType() const
{
    return writerType;
}

bool preferences::BasePreferenceWriter::write(const std::string path,
                                              const std::unique_ptr<preferences::PreferencesModel> &model)
{
    bool result = false;

    auto oss = std::make_unique<std::ostringstream>();

    result = writeModel(*oss, model);

    oss->flush();

    qWarning() << "Current string values." << oss->str().c_str();

    bool ifShowError = false;

    auto showMessageFunction = [&path]() { Q_UNUSED(path); };

    try
    {
        if (QString::fromStdString(path).startsWith("smb://"))
        {
            gpui::smb::SmbFile smbLocationItemFile(QString::fromStdString(path));
            ifShowError = smbLocationItemFile.open(QFile::WriteOnly | QFile::Truncate);
            if (!ifShowError)
            {
                result = ifShowError = smbLocationItemFile.open(QFile::NewOnly | QFile::WriteOnly);
            }
            if (ifShowError && oss->str().size() > 0)
            {
                smbLocationItemFile.write(&oss->str().at(0), oss->str().size());
            }
            smbLocationItemFile.close();
        }
        else
        {
            QFile registryFile(QString::fromStdString(path));
            ifShowError = registryFile.open(QFile::WriteOnly | QFile::Truncate);
            if (!ifShowError)
            {
                result = ifShowError = registryFile.open(QFile::NewOnly | QFile::WriteOnly);
            }
            if (ifShowError && registryFile.isWritable() && oss->str().size() > 0)
            {
                registryFile.write(&oss->str().at(0), oss->str().size());
            }
            registryFile.close();
        }
    }
    catch (std::exception &e)
    {
        ifShowError = true;
        showMessageFunction();
    }

    if (!ifShowError)
    {
        showMessageFunction();
    }

    return result;
}

preferences::BasePreferenceWriter::BasePreferenceWriter(const std::string &type)
    : writerType(type)
{}
