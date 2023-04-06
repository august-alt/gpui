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
#include <QDebug>
#include <QDirIterator>
#include <QLibraryInfo>

TranslatorStorage::TranslatorStorage()
    : d(std::make_unique<TranslatorStoragePrivate>())
    , m_errorString("")
{}

bool TranslatorStorage::loadTranslators(const QString &language)
{
    return loadTranslators(language, ":/");
}

bool TranslatorStorage::loadTranslators(const QString &language, const QString &path)
{
    auto languageToLoad = language.split("-").at(0);

    bool loadResult = false;

    QDirIterator it(path, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString currentFilename = it.fileName();

        if (!it.fileInfo().isFile())
        {
            it.next();
        }

        if (currentFilename.endsWith(languageToLoad + ".qm"))
        {
            std::unique_ptr<QTranslator> translator = std::make_unique<QTranslator>();
            bool currentLoadResult                  = translator->load(it.fileName(), path);

            if (currentLoadResult)
            {
                loadResult = true;
                QCoreApplication::installTranslator(translator.get());
                d->translators.push_back(std::move(translator));
            }

            else
            {
                setErrorString("WARNING! Can't load translate from file: " + currentFilename);

                return false;
            }
        }

        it.next();
    }

    return loadResult;
}

bool TranslatorStorage::loadQtTranslations(const QString &language, const QString &prefix)
{
    return loadTranslators(QString(prefix + "%1").arg(language),
                           QLibraryInfo::location(QLibraryInfo::TranslationsPath));
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
    m_errorString = error;
}

QString TranslatorStorage::getErrorString() const
{
    return m_errorString;
}
