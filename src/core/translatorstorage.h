#ifndef TRANSLATORSTORAGE_H
#define TRANSLATORSTORAGE_H

#include "core.h"
#include <memory>

#include "translatorstorageprivate.h"

class GPUI_CORE_EXPORT TranslatorStorage
{
public:
    /**
    * Loads a list of translation files from resources that end with the name of the language from the current directory
    */

    bool loadAndInstallTranslators(const QString &language);

    /**
    * Loads a list of translation files from resources that end with the name of the language from the specified directory
    */
    bool loadAndInstallTranslators(const QString &language, const QString &path);

    /**
    * Removes and uninstalls all translations
    */
    void clearAndUnistallTranslators();

    static TranslatorStorage *instance();

private:
    TranslatorStorage();

private:
    std::unique_ptr<TranslatorStoragePrivate> d;

private:
    TranslatorStorage(const TranslatorStorage &) = delete;
    TranslatorStorage(TranslatorStorage &&)      = delete;
    TranslatorStorage &operator=(const TranslatorStorage &) = delete;
    TranslatorStorage &operator=(TranslatorStorage &&) = delete;
};

#endif // TRANSLATORSTORAGE_H
