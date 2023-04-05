#include "translatorstorage.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDirIterator>
#include <QLibraryInfo>

TranslatorStorage::TranslatorStorage()
    : d(std::make_unique<TranslatorStoragePrivate>())
{}

bool TranslatorStorage::loadAndInstallTranslators(const QString &language)
{
    return loadAndInstallTranslators(language, ":/");
}

bool TranslatorStorage::loadAndInstallTranslators(const QString &language, const QString &path)
{
    auto languageToLoad = language.split("-").at(0);

    bool loadResult = false;

    QDirIterator it(path, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        if (!it.fileInfo().isFile())
        {
            it.next();
        }

        if (it.fileName().endsWith(languageToLoad + ".qm"))
        {
            std::unique_ptr<QTranslator> translator = std::make_unique<QTranslator>();
            loadResult                              = translator->load(it.fileName(), path);

            if (loadResult)
            {
                QCoreApplication::installTranslator(translator.get());
                d->translators.push_back(std::move(translator));
            }
        }

        it.next();
    }

    return loadResult;
}

bool TranslatorStorage::loadAndInstallQtTranslations(const QString &language, const QString &prefix)
{
    return loadAndInstallTranslators(QString(prefix).arg(language),
                                     QLibraryInfo::location(QLibraryInfo::TranslationsPath));
}

void TranslatorStorage::clearAndUnistallTranslators()
{
    for (const auto &translator : d->translators)
    {
        QCoreApplication::removeTranslator(translator.get());
    }

    d->translators.clear();
}

TranslatorStorage *TranslatorStorage::instance()
{
    static TranslatorStorage storage;

    return &storage;
}
