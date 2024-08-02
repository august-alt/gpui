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

#include "../plugins/storage/smb/smbfile.h"

#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>

#include <fstream>

namespace comments
{

struct CommentWithNamespace
{
    QString first{};
    QString second{};
    QString namespace_{};
};

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

template<typename TFileFormat, typename TComment>
void savePolicies(const QString &pluginName, const QString &fileName, std::shared_ptr<TComment>& comments)
{
    std::unique_ptr<TFileFormat> fileData = std::make_unique<TFileFormat>();
    fileData->add(comments);

    io::PolicyFileFormat<TFileFormat> *format = gpui::PluginStorage::instance()->createPluginClass<io::PolicyFileFormat<TFileFormat>>(pluginName);

    if (!format)
    {
        qWarning() << "Format supporting: " << pluginName << " not found.";

        return;
    }

    auto oss = std::make_unique<std::ostringstream>();

    if (!format->write(*oss, fileData.get()))
    {
        qWarning() << fileName << " " << format->getErrorString().c_str();
    }

    oss->flush();

    qWarning() << "Current string values:\n" << oss->str().c_str();

    bool ifShowError = false;

    auto showMessageFunction = [&fileName]() {
        QMessageBox messageBox(QMessageBox::Critical,
                               QObject::tr("Error"),
                               QObject::tr("Error writing file:") + "\n" + fileName,
                               QMessageBox::Ok);
        messageBox.exec();
    };

    try
    {
        if (fileName.startsWith("smb://"))
        {
            gpui::smb::SmbFile smbLocationItemFile(fileName);
            ifShowError = smbLocationItemFile.open(QFile::WriteOnly | QFile::Truncate);
            if (!ifShowError)
            {
                ifShowError = smbLocationItemFile.open(QFile::NewOnly | QFile::WriteOnly);
            }
            if (ifShowError && oss->str().size() > 0)
            {
                smbLocationItemFile.write(&oss->str().at(0), oss->str().size());
            }
            smbLocationItemFile.close();
        }
        else
        {
            QFile registryFile(fileName);
            ifShowError = registryFile.open(QFile::WriteOnly | QFile::Truncate);
            if (!ifShowError)
            {
                ifShowError = registryFile.open(QFile::NewOnly | QFile::WriteOnly);
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

    delete format;
}

QString constructCMTLFileName(const QFileInfo &fileName)
{
    QString admlFileName = fileName.filePath();
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

    QMap<QString, QString> namespaces;

    for (const auto& policyNamespace : commentDefinitions->getPolicyComments(0)->policyNamespaces.using_)
    {
        namespaces.insert(QString::fromStdString(policyNamespace.prefix), QString::fromStdString(policyNamespace.namespace_));
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

        auto key = QString::fromStdString(comment.policyRef).split(":").first();

        QString namespace_ = namespaces[key];

        QStandardItem* item = new QStandardItem(QString::fromStdString(resourceRef));
        auto policyRef = constructPolicyRef(comment.policyRef);
        item->setData(QString::fromStdString(policyRef), CommentsModel::ITEM_REFERENCE_ROLE);
        item->setData(namespace_, CommentsModel::ITEM_NAMESPACE_ROLE);

        qWarning() << comment.commentText.c_str() << comment.policyRef.c_str();

        this->appendRow(item);
    }
}

void CommentsModel::save(const QString &path, const QString& localeName)
{
    qWarning() << "Imitating write of cmtx and cmtl to: " << path;

    std::shared_ptr<comments::PolicyComments> commentDefinitions = std::make_shared<comments::PolicyComments>();

    QSet<QString> namespaces;
    QVector<CommentWithNamespace> comments;

    auto current = invisibleRootItem()->index();

    for (int row = 0; row < this->rowCount(current); ++row)
    {
        QModelIndex index = this->index(row, 0, current);

        namespaces.insert(index.data(CommentsModel::ITEM_NAMESPACE_ROLE).value<QString>());
        comments.append({
                            index.data(CommentsModel::ITEM_REFERENCE_ROLE).value<QString>(),
                            index.data().value<QString>(),
                            index.data(CommentsModel::ITEM_NAMESPACE_ROLE).value<QString>()
                        });
    }

    if (comments.empty())
    {
        return;
    }

    int namespaceIndex = 0;

    for (const auto& namespace_ : namespaces)
    {
        commentDefinitions->policyNamespaces.using_.emplace_back("ns" + std::to_string(namespaceIndex),
                                                                namespace_.toStdString());
        namespaceIndex++;
    }

    commentDefinitions->resources = std::make_unique<LocalizationResourceReference>();

    bool enableLocalizedComments = localeName != "en-US";

    for (const auto& comment : comments)
    {
        namespaceIndex = 0;

        for (const auto& currentNamespace : commentDefinitions->policyNamespaces.using_)
        {
            if (comment.namespace_.compare(currentNamespace.namespace_.c_str()) == 0)
            {
                break;
            }

            namespaceIndex++;
        }

        std::string resourceName = "ns" + std::to_string(namespaceIndex) + "_" + comment.first.toStdString();

        comments::Comment currentComment;
        currentComment.policyRef = "ns" + std::to_string(namespaceIndex) + ":" + comment.first.toStdString();
        currentComment.commentText = "$(resource." + resourceName + ")";

        commentDefinitions->comments.push_back(currentComment);

        commentDefinitions->resources->stringTable.emplace_back((enableLocalizedComments
                                                                ? ""
                                                                : comment.second.toStdString()),
                                                                resourceName);
    }

    if (enableLocalizedComments)
    {
        auto cmtlFileName = path + "comment.cmtl";

        std::shared_ptr<comments::CommentDefinitionResources> commentResources
                = std::make_shared<comments::CommentDefinitionResources>();

        for (const auto& comment : comments)
        {
            namespaceIndex = 0;

            for (const auto& currentNamespace : commentDefinitions->policyNamespaces.using_)
            {
                if (comment.namespace_.compare(currentNamespace.namespace_.c_str()) == 0)
                {
                    break;
                }

                namespaceIndex++;
            }

            commentResources->stringTable.emplace_back(comment.second.toStdString(),
                                                       "ns" + std::to_string(namespaceIndex)
                                                       + "_" + comment.first.toStdString());
        }

        savePolicies<io::CommentResourcesFile, comments::CommentDefinitionResources>("cmtl", cmtlFileName,
                                                                                     commentResources);
    }

    auto cmtxFileName = path + "comment.cmtx";

    savePolicies<io::PolicyCommentsFile, comments::PolicyComments>("cmtx", cmtxFileName, commentDefinitions);
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

bool CommentsModel::setComment(const QString &comment, const QString& policyName, const QString &namespace_)
{
    auto current = invisibleRootItem()->index();
    bool commentFound = false;

    for (int row = 0; row < this->rowCount(current); ++row)
    {
        QModelIndex index = this->index(row, 0, current);

        auto itemReference = index.data(CommentsModel::ITEM_REFERENCE_ROLE).value<QString>();
        auto itemNamespace = index.data(CommentsModel::ITEM_NAMESPACE_ROLE).value<QString>();

        if (itemReference.compare(policyName) == 0
            && itemNamespace.compare(namespace_) == 0)
        {
            qWarning() << "Changing comment in model: " << index << comment;

            setData(index, comment);
            commentFound = true;
            break;
        }
    }

    if (!commentFound)
    {
        QStandardItem* item = new QStandardItem(comment);
        item->setData(policyName, CommentsModel::ITEM_REFERENCE_ROLE);
        item->setData(namespace_, CommentsModel::ITEM_NAMESPACE_ROLE);

        appendRow(item);

        qWarning() << "Appending comment to model: " <<  item;
    }

    return true;
}

}
