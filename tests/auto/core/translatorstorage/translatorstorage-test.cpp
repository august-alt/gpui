#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory.h>
#include <QApplication>
#include <QObject>
#include <QTest>

#include "../../../../src/core/translatorstorage.h"
#include "translatorstorageshadow.h"

TEST(TranslatorStorage, loadTranslators)
{
    QObject::tr("test");

    TranslatorStorage translatorStorage;
    translatorStorage.loadTranslators("en");

    void *ptr                                 = &translatorStorage;
    tests::TranslatorStorageShadow *shadowPtr = static_cast<tests::TranslatorStorageShadow *>(ptr);
    auto loadedTranslations                   = shadowPtr->d.get()->translators.size();

    EXPECT_EQ(loadedTranslations, 1);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
