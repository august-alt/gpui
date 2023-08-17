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

#include "translatorstorage.h"

#include <QCoreApplication>
#include "logger/log.h"
#include <QDirIterator>
#include <QLibraryInfo>

class TranslatorStoragePrivate
{
public:
    std::vector<std::unique_ptr<QTranslator>> translators = {};
    QString m_errorString = {};
};

TranslatorStorage::TranslatorStorage()
    : d(new TranslatorStoragePrivate())
{}

TranslatorStorage::~TranslatorStorage()
{}

bool TranslatorStorage::loadTranslators(const QString &language)
{
    return loadTranslators(language, ":/");
}

bool TranslatorStorage::loadTranslators(const QString &language, const QString &path)
{
    QString languageToLoad = language.split("-").at(0);

    QDir dir(path);

    for (auto &entry : dir.entryList())
    {
        if (entry.endsWith(languageToLoad + ".qm"))
        {
            std::unique_ptr<QTranslator> translator = std::make_unique<QTranslator>();

            if (translator->load(entry, path))
            {
                QCoreApplication::installTranslator(translator.get());
                d->translators.push_back(std::move(translator));
            }
            else
            {
                setErrorString("WARNING! Can't load translate from file: " + entry);

                return false;
            }
        }
    }

    return true;
}

bool TranslatorStorage::loadQtTranslations(const QString &language, const QString &prefix)
{
    return loadTranslators(QString(prefix + "%1").arg(language), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
}

void TranslatorStorage::clearTranslators()
{
    for (const auto &translator : d->translators)
    {
        QCoreApplication::removeTranslator(translator.get());
    }

    d->translators.clear();
}

void TranslatorStorage::setErrorString(const QString &error)
{
    d->m_errorString = error;
}

QString TranslatorStorage::getErrorString() const
{
    return d->m_errorString;
}
