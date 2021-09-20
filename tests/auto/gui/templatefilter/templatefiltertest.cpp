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

#include "templatefiltertest.h"

#include "../../../../src/gui/templatefilter.h"
#include "../../../../src/model/bundle/policyroles.h"
#include "../../../../src/gui/templatefiltermodel.h"

#include <QStandardItemModel>
#include <QStandardItem>

// TODO: test comment text filtering (when it's implemented)

namespace tests {

using namespace gpui;
using namespace model::admx;
using namespace model::registry;
using namespace model::bundle;

void TemplateFilterTest::filterAcceptsRow_data()
{
    QTest::addColumn<TemplateFilter>("filter");
    QTest::addColumn<bool>("filterEnabled");
    QTest::addColumn<bool>("isTemplate");
    QTest::addColumn<QString>("titleText");
    QTest::addColumn<QString>("helpText");
    QTest::addColumn<QString>("commentText");
    QTest::addColumn<int>("policyStateInt");
    QTest::addColumn<bool>("expectedReturn");

    QTest::newRow("filter disabled") << TemplateFilter() << false << false << "" << "" << "" << 0 << true;
    QTest::newRow("not template") << TemplateFilter() << true << false << "" << "" << "" << 0 << true;

    {
        TemplateFilter filter = {0};
        filter.configured = QSet<PolicyStateManager::PolicyState>({
            PolicyStateManager::STATE_NOT_CONFIGURED,
            PolicyStateManager::STATE_ENABLED,
            PolicyStateManager::STATE_DISABLED,
        });

        QTest::newRow("configured yes, accept enabled") << filter << true << true << "" << "" << "" << (int) PolicyStateManager::STATE_ENABLED << true;
        QTest::newRow("configured yes, accept disabled") << filter << true << true << "" << "" << "" << (int) PolicyStateManager::STATE_DISABLED << true;
        QTest::newRow("configured yes, accept not configured") << filter << true << true << "" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << true;
    }

    {
        TemplateFilter filter = {0};
        filter.configured = QSet<PolicyStateManager::PolicyState>({
            PolicyStateManager::STATE_ENABLED,
            PolicyStateManager::STATE_DISABLED,
        });

        QTest::newRow("configured yes, accept enabled") << filter << true << true << "" << "" << "" << (int) PolicyStateManager::STATE_ENABLED << true;
        QTest::newRow("configured yes, accept disabled") << filter << true << true << "" << "" << "" << (int) PolicyStateManager::STATE_DISABLED << true;
        QTest::newRow("configured yes, reject not configured") << filter << true << true << "" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << false;
    }

    {
        TemplateFilter filter = {0};
        filter.configured = QSet<PolicyStateManager::PolicyState>({
            PolicyStateManager::STATE_NOT_CONFIGURED,
        });

        QTest::newRow("configured no, reject enabled") << filter << true << true << "" << "" << "" << (int) PolicyStateManager::STATE_ENABLED << false;
        QTest::newRow("configured no, reject disabled") << filter << true << true << "" << "" << "" << (int) PolicyStateManager::STATE_DISABLED << false;
        QTest::newRow("configured no, accept not configured") << filter << true << true << "" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << true;
    }

    {
        TemplateFilter filter = {0};
        filter.keywordEnabled = true;
        filter.titleEnabled = true;
        filter.keywordType = KeywordFilterType_ANY;
        filter.keywordText = "hello sailor";
        filter.configured = QSet<PolicyStateManager::PolicyState>({
            PolicyStateManager::STATE_NOT_CONFIGURED,
        });

        QTest::newRow("filter any, accept hello") << filter << true << true << "hello" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << true;
        QTest::newRow("filter any, accept sailor") << filter << true << true << "sailor" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << true;
        QTest::newRow("filter any, reject modernity") << filter << true << true << "modernity" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << false;
    }

    {
        TemplateFilter filter = {0};
        filter.keywordEnabled = true;
        filter.titleEnabled = true;
        filter.keywordType = KeywordFilterType_EXACT;
        filter.keywordText = "hello sailor";
        filter.configured = QSet<PolicyStateManager::PolicyState>({
            PolicyStateManager::STATE_NOT_CONFIGURED,
        });

        QTest::newRow("filter exact, accept") << filter << true << true << "hello sailor" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << true;
        QTest::newRow("filter exact, reject") << filter << true << true << "goodbye sailor" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << false;
    }

    {
        TemplateFilter filter = {0};
        filter.keywordEnabled = true;
        filter.titleEnabled = true;
        filter.keywordType = KeywordFilterType_ALL;
        filter.keywordText = "hello sailor";
        filter.configured = QSet<PolicyStateManager::PolicyState>({
            PolicyStateManager::STATE_NOT_CONFIGURED,
        });

        QTest::newRow("filter all, accept same string") << filter << true << true << "hello sailor" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << true;
        QTest::newRow("filter all, accept extra word") << filter << true << true << "hello brave sailor" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << true;
        QTest::newRow("filter all, reject missing word") << filter << true << true << "goodbye sailor" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << false;
    }

    {
        TemplateFilter filter = {0};
        filter.keywordEnabled = true;
        filter.titleEnabled = true;
        filter.keywordType = KeywordFilterType_EXACT;
        filter.keywordText = "hello sailor";
        filter.configured = QSet<PolicyStateManager::PolicyState>({
            PolicyStateManager::STATE_NOT_CONFIGURED,
        });

        QTest::newRow("filter exact, reject mismatch") << filter << true << true << "goodbye sailor" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << false;

        filter.titleEnabled = true;
        filter.keywordEnabled = false;
        QTest::newRow("filter exact, accept mismatch because keyword filter is disabled") << filter << true << true << "goodbye sailor" << "" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << true;
    }

    {
        TemplateFilter filter = {0};
        filter.keywordEnabled = true;
        filter.titleEnabled = true;
        filter.helpEnabled = true;
        filter.keywordType = KeywordFilterType_EXACT;
        filter.keywordText = "hello sailor";
        filter.configured = QSet<PolicyStateManager::PolicyState>({
            PolicyStateManager::STATE_NOT_CONFIGURED,
        });

        QTest::newRow("filter exact, accept title, help and comment") << filter << true << true << "hello sailor" << "hello sailor" << "hello sailor" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << true;
        QTest::newRow("filter exact, accept partial match of title/comment") << filter << true << true << "hello sailor" << "goodbye sailor" << "hello sailor" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << true;
        QTest::newRow("filter exact, reject no match") << filter << true << true << "goodbye sailor" << "goodbye sailor" << "" << (int) PolicyStateManager::STATE_NOT_CONFIGURED << false;
    }
}

void TemplateFilterTest::filterAcceptsRow()
{
    QFETCH(TemplateFilter, filter);
    QFETCH(bool, filterEnabled);
    QFETCH(bool, isTemplate);
    QFETCH(QString, titleText);
    QFETCH(QString, helpText);
    QFETCH(QString, commentText);
    QFETCH(int, policyStateInt);
    const PolicyStateManager::PolicyState policyState = (PolicyStateManager::PolicyState) policyStateInt;
    QFETCH(bool, expectedReturn);

    auto sourceModel = new QStandardItemModel(this);

    auto filterModel = new TemplateFilterModel(this);
    filterModel->setSourceModel(sourceModel);

    filterModel->setFilter(filter, filterEnabled);

    auto item = new QStandardItem();

    const int itemTypeValue = [&]()
    {
        if (isTemplate) {
            return 1;
        } else {
            return 0;
        }
    }();
    item->setData(itemTypeValue, PolicyRoles::ITEM_TYPE);
    item->setData(titleText, Qt::DisplayRole);
    item->setData(helpText, PolicyRoles::EXPLAIN_TEXT);
    item->setData(commentText, PolicyRoles::EXPLAIN_TEXT);

    sourceModel->appendRow(item);

    const bool actualReturn = filterModel->filterAcceptsRow(item->index(), policyState);

    QCOMPARE(actualReturn, expectedReturn);
}

}

QTEST_MAIN(tests::TemplateFilterTest)
