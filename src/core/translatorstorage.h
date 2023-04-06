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

#ifndef GPUI_TRANSLATOR_STORAGE_H
#define GPUI_TRANSLATOR_STORAGE_H

#include "core.h"

#include <memory>
#include <QString>
#include <QTranslator>

class TranslatorStoragePrivate;

/**
 * @brief The TranslatorStorage class provides for loading and storing translation files.
 */
class GPUI_CORE_EXPORT TranslatorStorage
{
public:
    TranslatorStorage();
    ~TranslatorStorage();

    /**
    * @brief loads and install a list of translation files from resources that end with the name of the language from the current directory
    * @param language to be loaded
    */

    bool loadTranslators(const QString &language);

    /**
    * @brief loads and installs a list of translation files from resources that end with the name of the language from the specified directory
    * @param language to be loaded
    * @param path of translation file to be loaded
    */
    bool loadTranslators(const QString &language, const QString &path);

    /**
    * @brief loads and installs a list of translation qt files from resources that begins with prefix and end with the name of the language in current directory
    * @param language to be loaded
    * @param prefix of the file name to be loaded
    */
    bool loadQtTranslations(const QString &language, const QString &prefix);

    /**
    * @brief removes and uninstalls all translations
    */
    void clearTranslators();

    /**
    * @brief sets a control error string that might be reused in a different context
    * @param error string
    */
    void setErrorString(const QString &error);

    /**
    * @brief gets a control error string that might be reused in a different context
    */
    QString getErrorString() const;

private:
    std::unique_ptr<TranslatorStoragePrivate> d;

private:
    TranslatorStorage(const TranslatorStorage &) = delete;
    TranslatorStorage(TranslatorStorage &&)      = delete;
    TranslatorStorage &operator=(const TranslatorStorage &) = delete;
    TranslatorStorage &operator=(TranslatorStorage &&) = delete;
};

#endif // GPUI_TRANSLATOR_STORAGE_H
