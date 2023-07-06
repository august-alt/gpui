/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#include "commentsmodel.h"

#include "../core/pluginstorage.h"

#include "../io/commentresourcesfile.h"
#include "../io/policycommentsfile.h"

#include "policycomments.h"
#include "commentdefinitionresources.h"

#include <QDebug>
#include <QFileInfo>

#include <fstream>

namespace comments
{

template<typename TPolicies, typename TFormat>
std::unique_ptr<TPolicies> loadPolicies(const QString &pluginName, const QFileInfo &policyFileName)
{
    std::unique_ptr<TPolicies> policies;

    TFormat *format = gpui::PluginStorage::instance()->createPluginClass<TFormat>(pluginName);

    if (!format)
    {
        return policies;
    }

    std::ifstream file;

    file.open(policyFileName.absoluteFilePath().toStdString(), std::ifstream::in);

    if (file.good())
    {
        policies = std::make_unique<TPolicies>();

        if (!format->read(file, policies.get()))
        {
            qWarning() << policyFileName.fileName() + " " + QString::fromStdString(format->getErrorString());
        }
    }

    file.close();

    delete format;

    return policies;
}

QString constructCMTLFileName(const QFileInfo &fileName)
{
    QString admlFileName = fileName.fileName();
    admlFileName.replace(admlFileName.length() - 4, 4, "cmtl");

    return admlFileName;
}

CommentsModel::CommentsModel(QObject *parent)
    : QStandardItemModel(parent)
{
}

std::string constructResourceRef(const std::string& id)
{
    if (id.length() < 12 || id.compare(0, 11, "$(resource.") != 0)
    {
        return id;
    }
    return id.substr(11, id.length() - 12);
}

std::string constructPolicyRef(const std::string& id)
{
    if (id.length() < 3 || id.compare(0, 2, "ns") != 0)
    {
        return id;
    }

    return id.substr(4);
}

void CommentsModel::load(const QString &cmtxFileName)
{
    auto commentDefinitions
        = loadPolicies<io::PolicyCommentsFile, io::PolicyFileFormat<io::PolicyCommentsFile>>("cmtx", cmtxFileName);
    if (!commentDefinitions.get())
    {
        qWarning() << "File not found: " << cmtxFileName;
        return;
    }

    bool noCMTL = false;
    QString cmtlFileName = constructCMTLFileName(cmtxFileName);
    auto commentTranslations
        = loadPolicies<io::CommentResourcesFile, io::PolicyFileFormat<io::CommentResourcesFile>>("cmtl", cmtlFileName);
    if (!commentTranslations.get())
    {
        qWarning() << "File not found: " << cmtlFileName;
        noCMTL = true;
    }
    else if (commentTranslations->commentResourcesCount() == 0)
    {
        noCMTL = true;
    }

    if (commentDefinitions->policyCommentsCount() == 0)
    {
        qWarning() << "No comment definitions in file: " << cmtxFileName;
        return;
    }

    for (const auto& comment : commentDefinitions->getPolicyComments(0)->comments)
    {
        auto resourceRef = constructResourceRef(comment.commentText);
        auto stringTable = noCMTL
                ? commentDefinitions->getPolicyComments(0)->resources->stringTable
                : commentTranslations->getCommentResources(0)->stringTable;

        for (const auto& resource : stringTable)
        {
            if (resource.first.compare(resourceRef) == 0)
            {
                resourceRef = resource.second;
            }
        }

        QStandardItem* item = new QStandardItem(QString::fromStdString(resourceRef));
        auto policyRef = constructPolicyRef(comment.policyRef);
        item->setData(QString::fromStdString(policyRef), CommentsModel::ITEM_REFERENCE_ROLE);

        qWarning() << comment.commentText.c_str() << comment.policyRef.c_str();

        this->appendRow(item);
    }
}

QModelIndex CommentsModel::indexFromItemReference(const QString &itemRef)
{
    auto current = invisibleRootItem()->index();

    for (int row = 0; row < this->rowCount(current); ++row)
    {
        QModelIndex index = this->index(row, 0, current);

        if (index.data(CommentsModel::ITEM_REFERENCE_ROLE).value<QString>().compare(itemRef) == 0)
        {
            return index;
        }
    }

    return QModelIndex();
}

}
