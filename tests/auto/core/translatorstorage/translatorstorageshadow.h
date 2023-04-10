#ifndef GPUI_TRANSLATOR_STORAGE_SHADOW_H
#define GPUI_TRANSLATOR_STORAGE_SHADOW_H

#include "../../../../src/core/translatorstorage.h"

namespace tests
{
class TranslatorStorageShadowPrivate;

class TranslatorStorageShadow
{
public:
    TranslatorStorageShadow();
    ~TranslatorStorageShadow();

public:
    std::unique_ptr<TranslatorStorageShadowPrivate> d;
};

class TranslatorStorageShadowPrivate
{
public:
    std::vector<std::unique_ptr<QTranslator>> translators = {};
    QString m_errorString                                 = {};
};
} // namespace tests
#endif
