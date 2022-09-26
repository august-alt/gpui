/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd. <org@basealt.ru>
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

#ifndef GPUI_FOLDERITEM_H
#define GPUI_FOLDERITEM_H

#include <mvvm/model/compounditem.h>

namespace preferences
{

//! Folder item representation for editor.

class FolderItem : public ModelView::CompoundItem
{
public:
    static inline const std::string ACTION = "action";
    static inline const std::string PATH = "path";
    static inline const std::string READONLY = "readonly";
    static inline const std::string ARCHIVE = "archive";
    static inline const std::string HIDDEN = "hidden";
    static inline const std::string DELETE_IGNORE_ERRORS = "deleteIgnoreErrors";
    static inline const std::string DELETE_FILES = "deleteFiles";
    static inline const std::string DELETE_SUB_FOLDERS = "deleteSubFolders";
    static inline const std::string DELETE_FOLDER = "deleteFolder";
    static inline const std::string DELETE_READ_ONLY = "deleteReadOnly";

    FolderItem();
    FolderItem(const FolderItem &other);

    QString action() const;
    void setAction(QString action);

    QString path() const;
    void setPath(const QString& path);

    bool readOnly() const;
    void setReadOnly(bool state);

    bool archive() const;
    void setArchive(bool state);

    bool hidden() const;
    void setHidden(bool state);

    bool deleteIgnoreErrors() const;
    void setDeleteIgnoreErrors(bool state);

    bool deleteFiles() const;
    void setDeleteFiles(bool state);

    bool deleteSubFolders() const;
    void setDeleteSubFolders(bool state);

    bool deleteFolder() const;
    void setDeleteFolder(bool state);
};

}

Q_DECLARE_METATYPE(::preferences::FolderItem)

#endif//GPUI_FOLDERITEM_H
