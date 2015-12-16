/*
* Copyright (C): 2015 Department of Software Engineering, University of Szeged
*
* Authors: David Havas <havasd@inf.u-szeged.hu>
*
* This file is part of SoDA.
*
*  SoDA is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  SoDA is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with SoDA.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "gtest/gtest.h"
#include "data/CBugset.h"
#include "data/CIDManager.h"
#include "exception/CException.h"

#include <sstream>

using namespace soda;

class CBugsetTest : public testing::Test
{
protected:
    CBugset *bugset;
    CBugset *bugset2;
    StringVector ceNames;
    StringVector revNames;
    IIDManager *ceIds;
    IIDManager *revIds;
    ReportDataMap *data;
    ReportMap *reports;

    virtual void SetUp() {
        // code elements
        for (int i = 0; i < 10; ++i) {
            std::stringstream ss;
            ss << "codeElements-" << i;
            ceNames.push_back(ss.str());
        }
        ceIds = new CIDManager(ceNames);

        // revisions, reports, reportsmap
        for (int i = 0; i < 10; ++i) {
            std::stringstream ss;
            ss << "rev-" << i;
            revNames.push_back(ss.str());
        }
        revIds = new CIDManager(revNames);

        data = new ReportDataMap();
        (*data)[1] = { 132, 642 };
        (*data)[2] = { 200, 842 };
        reports = new ReportMap();
        (*reports)[1][1] = 1;
        bugset = new CBugset();
        bugset2 = new CBugset(ceIds, revIds, reports, data);
    }

    virtual void TearDown() {
        delete bugset;
        delete bugset2;
        delete reports;
        delete data;
        delete ceIds;
        delete revIds;
    }

};

TEST_F(CBugsetTest, ArrayIndexOperator) {
    CodeElementReports& rep = (*bugset2)["rev-1"];
    EXPECT_EQ(1, rep.size());
}

TEST_F(CBugsetTest, At) {
    EXPECT_NO_THROW(bugset2->at("rev-1"));
    EXPECT_ANY_THROW(bugset->at("rev-1"));
}

TEST_F(CBugsetTest, Exists) {
    EXPECT_TRUE(bugset2->exists("rev-1"));
    EXPECT_FALSE(bugset->exists("rev-1"));
}

TEST_F(CBugsetTest, ContainsData) {
    EXPECT_TRUE(bugset2->containsData("rev-1", "codeElements-1"));
    EXPECT_FALSE(bugset2->containsData("rev-1", "codeElements-11"));
    EXPECT_FALSE(bugset2->containsData("rev-11", "codeElements-1"));
    EXPECT_FALSE(bugset2->containsData("rev-11", "codeElements-11"));
}

TEST_F(CBugsetTest, GetReportsInformation) {
    Report const &rep = bugset2->getReportInformations("rev-1", "codeElements-1");
    EXPECT_EQ(642, rep.fixTime);
    EXPECT_EQ(132, rep.reportTime);
    // not existing
    EXPECT_THROW(Report const &noRep = bugset2->getReportInformations("rev-11", "codeElements-11"), CException);
    EXPECT_THROW(Report const &noRep = bugset2->getReportInformations("rev-1", "codeElements-11"), CException);
    EXPECT_THROW(Report const &noRep = bugset2->getReportInformations("rev-11", "codeElements-1"), CException);
}

TEST_F(CBugsetTest, GetReports) {
    EXPECT_EQ(2, bugset2->getReports().size());
    EXPECT_EQ(1, bugset2->getReports().count(1));
    EXPECT_EQ(1, bugset2->getReports().count(2));
    EXPECT_EQ(0, bugset2->getReports().count(3));
}

TEST_F(CBugsetTest, GetReportMap) {
    EXPECT_EQ(1, bugset2->getReportMap().size());
    EXPECT_EQ(1, bugset2->getReportMap().count(1));
    EXPECT_EQ(0, bugset2->getReportMap().count(2));
}

TEST_F(CBugsetTest, AddReported) {
    EXPECT_FALSE(bugset2->containsData("rev-1", "codeElements-23"));
    Report data{ 1232, 2412124124 };
    std::string rev = "rev-1";
    std::string ce = "codeElements-23";
    EXPECT_NO_FATAL_FAILURE(bugset2->addReported(rev, ce, 3, data));
    EXPECT_TRUE(bugset2->containsData("rev-1", "codeElements-23"));
}

TEST_F(CBugsetTest, Save) {
    EXPECT_NO_FATAL_FAILURE(bugset2->save("sample/bugtest.saved"));
}

TEST_F(CBugsetTest, Load) {
    CBugset tmp;
    EXPECT_FALSE(tmp.containsData("rev-1", "codeElements-1"));
    EXPECT_NO_FATAL_FAILURE(tmp.load("sample/bugtest.saved"));
    EXPECT_TRUE(tmp.containsData("rev-1", "codeElements-1"));
}

TEST_F(CBugsetTest, Constructor)
{
    EXPECT_NO_THROW(CBugset tmp);
    EXPECT_NO_THROW(CBugset tmp2(ceIds));

    CBugset *tmp3;
    EXPECT_NO_THROW(tmp3 = new CBugset(ceIds, revIds, reports, data));
    EXPECT_EQ(2, tmp3->getReports().size());
    EXPECT_EQ(1, tmp3->getReportMap().size());
    delete tmp3;
}

TEST_F(CBugsetTest, AddCodeElementsFromCIDManager) {
    EXPECT_EQ(0, bugset->getCodeElements().size());
    // CIDManager
    CIDManager ceIds(ceNames);
    EXPECT_NO_FATAL_FAILURE(bugset->addCodeElement(ceIds));
    EXPECT_EQ(10, bugset->getCodeElements().size());
}

TEST_F(CBugsetTest, AddSingleCodeElement) {
    EXPECT_EQ(0, bugset->getCodeElements().size());
    // single ce
    std::string ce = "testce";
    EXPECT_NO_FATAL_FAILURE(bugset->addCodeElementName(ce));
    EXPECT_EQ(1, bugset->getCodeElements().size());
}

TEST_F(CBugsetTest, AddCodeElementsFromStringVector) {
    EXPECT_EQ(0, bugset->getCodeElements().size());
    // StringVector
    EXPECT_NO_FATAL_FAILURE(bugset->addCodeElement(ceNames));
    EXPECT_EQ(10, bugset->getCodeElements().size());
}

TEST_F(CBugsetTest, RemoveCodeElementsOperations) {
    EXPECT_EQ(10, bugset2->getCodeElements().size());
    EXPECT_NO_FATAL_FAILURE(bugset2->removeCodeElement("codeElements-1"));
    EXPECT_EQ(9, bugset2->getCodeElements().size());
}

TEST_F(CBugsetTest, AddRevision) {
    EXPECT_EQ(0, bugset->getRevisions().size());
    // single rev
    std::string rev = "testrev";
    EXPECT_NO_FATAL_FAILURE(bugset->addRevision(rev));
    EXPECT_EQ(1, bugset->getRevisions().size());
}

TEST_F(CBugsetTest, AddMultipleRevision) {
    EXPECT_EQ(0, bugset->getRevisions().size());
    // StringVector
    EXPECT_NO_FATAL_FAILURE(bugset->addRevision(revNames));
    EXPECT_EQ(10, bugset->getRevisions().size());
}

TEST_F(CBugsetTest, RemoveRevisionOperations) {
    EXPECT_EQ(10, bugset2->getRevisions().size());
    EXPECT_NO_FATAL_FAILURE(bugset2->removeRevision("rev-1"));
    EXPECT_EQ(9, bugset2->getRevisions().size());
}
