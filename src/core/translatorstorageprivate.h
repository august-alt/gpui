#ifndef TRANSLATORSTORAGEPRIVATE_H
#define TRANSLATORSTORAGEPRIVATE_H

#include <memory>
#include <qtranslator.h>

class TranslatorStoragePrivate
{
public:
    TranslatorStoragePrivate();
    ~TranslatorStoragePrivate() = default;

    std::vector<std::unique_ptr<QTranslator>> translators;
};

#endif // TRANSLATORSTORAGEPRIVATE_H
