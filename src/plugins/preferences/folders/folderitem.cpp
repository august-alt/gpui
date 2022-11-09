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

#include "folderitem.h"

namespace preferences
{

FolderItem::FolderItem()
    : ModelView::CompoundItem("FolderItem")
{
    addProperty(ACTION, 0);
    addProperty(PATH, "");
    addProperty(READONLY, false);
    addProperty(ARCHIVE, false);
    addProperty(HIDDEN, false);
    addProperty(DELETE_IGNORE_ERRORS, false);
    addProperty(DELETE_FILES, false);
    addProperty(DELETE_SUB_FOLDERS, false);
    addProperty(DELETE_FOLDER, false);
    addProperty(DELETE_READ_ONLY, false);
}

FolderItem::FolderItem(const FolderItem &other)
    : ModelView::CompoundItem("FolderItem")
{
    addProperty(ACTION, other.action());
    addProperty(PATH, other.path());
    addProperty(READONLY, other.readOnly());
    addProperty(ARCHIVE, other.archive());
    addProperty(HIDDEN, other.hidden());
    addProperty(DELETE_IGNORE_ERRORS, other.deleteIgnoreErrors());
    addProperty(DELETE_FILES, other.deleteFiles());
    addProperty(DELETE_SUB_FOLDERS, other.deleteSubFolders());
    addProperty(DELETE_FOLDER, other.deleteFolder());
    addProperty(DELETE_READ_ONLY, other.property<bool>(DELETE_READ_ONLY));
}

QString FolderItem::action() const
{
    return property<QString>(ACTION);
}

void FolderItem::setAction(QString newAction)
{
    setProperty(ACTION, newAction);
}

QString FolderItem::path() const
{
    return property<QString>(PATH);
}

void FolderItem::setPath(const QString& newPath)
{
    setProperty(PATH, newPath);
}

bool FolderItem::readOnly() const
{
    return property<bool>(READONLY);
}

void FolderItem::setReadOnly(bool state)
{
    setProperty(READONLY, state);
}

bool FolderItem::archive() const
{
    return property<bool>(ARCHIVE);
}

void FolderItem::setArchive(bool state)
{
    setProperty(ARCHIVE, state);
}

bool FolderItem::hidden() const
{
    return property<bool>(HIDDEN);
}

void FolderItem::setHidden(bool state)
{
    setProperty(HIDDEN, state);
}

bool FolderItem::deleteIgnoreErrors() const
{
    return property<bool>(DELETE_IGNORE_ERRORS);
}

void FolderItem::setDeleteIgnoreErrors(bool state)
{
    setProperty(DELETE_IGNORE_ERRORS, state);
}

bool FolderItem::deleteFiles() const
{
    return property<bool>(DELETE_FILES);
}

void FolderItem::setDeleteFiles(bool state)
{
    setProperty(DELETE_FILES, state);
}

bool FolderItem::deleteSubFolders() const
{
    return property<bool>(DELETE_SUB_FOLDERS);
}

void FolderItem::setDeleteSubFolders(bool state)
{
    setProperty(DELETE_SUB_FOLDERS, state);
}

bool FolderItem::deleteFolder() const
{
    return property<bool>(DELETE_FOLDER);
}

void FolderItem::setDeleteFolder(bool state)
{
    setProperty(DELETE_FOLDER, state);
}

}
