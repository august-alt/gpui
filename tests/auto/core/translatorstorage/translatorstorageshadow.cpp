#include "translatorstorageshadow.h"

#include <memory>

namespace tests
{
TranslatorStorageShadow::TranslatorStorageShadow()
    : d(new TranslatorStorageShadowPrivate)
{}

TranslatorStorageShadow::~TranslatorStorageShadow() {}

} // namespace tests
