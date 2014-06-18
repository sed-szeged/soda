/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
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

#include "util/CSelectionStatistics.h"
#include <sstream>

using namespace std;

namespace soda {

CSelectionStatistics::CSelectionStatistics(CSelectionData *data) :
    m_selectionData(data)
{
}

CSelectionStatistics::~CSelectionStatistics()
{}


void CSelectionStatistics::calcCoverageRelatedStatistics(rapidjson::Document &doc)
{
    (cerr << "[INFO] Calculating coverage statistics ..." << endl).flush();

    IndexType nrOfCodeElements = m_selectionData->getCoverage()->getNumOfCodeElements();
    IndexType nrOfTestCases = m_selectionData->getCoverage()->getNumOfTestcases();
    IdxIdxMap dataCodeElements;
    IdxIdxMap dataTestCases;
    IntVector coveredCodeElements(nrOfCodeElements);
    float covered = 0;

    for (IndexType tcid = 0; tcid < nrOfTestCases; tcid++) {
        (cerr << "\r[INFO] Calculating coverage statistics ... " << (tcid + 1) << "/" << nrOfTestCases).flush();
        IndexType count = 0;
        for (IndexType ceid = 0; ceid < nrOfCodeElements; ceid++) {
            if (m_selectionData->getCoverage()->getBitMatrix().get(tcid, ceid)) {
                coveredCodeElements[ceid]++;
                count++;
            }
        }
        covered += count;
        dataTestCases[count]++;
    }

    for (IndexType ceid = 0; ceid < nrOfCodeElements; ceid++) {
        dataCodeElements[coveredCodeElements[ceid]]++;
    }

    (cerr << endl).flush();
    doc.AddMember("number_of_code_elements", nrOfCodeElements, doc.GetAllocator());
    doc.AddMember("number_of_test_cases", nrOfTestCases, doc.GetAllocator());
    doc.AddMember("density", covered / (nrOfTestCases * nrOfCodeElements), doc.GetAllocator());
    doc.AddMember("average_test_cases_per_code_elements", covered / nrOfCodeElements, doc.GetAllocator());
    doc.AddMember("average_code_elements_per_test_cases", covered / nrOfTestCases, doc.GetAllocator());
    rapidjson::Value testCov(rapidjson::kObjectType);
    toJson(dataTestCases, testCov, doc);
    doc.AddMember("test_coverage_histogram", testCov, doc.GetAllocator());
    rapidjson::Value codeCov(rapidjson::kObjectType);
    toJson(dataCodeElements, codeCov, doc);
    doc.AddMember("code_coverage_histogram", codeCov, doc.GetAllocator());
}

/*
void CSelectionStatistics::calcChangeRelatedStatistics()
{
    (cerr << "[INFO] Calculating calcChangeRelatedStatistics ..." << endl).flush();

    const IntVector revisions = m_selectionData->getChangeset()->getRevisions();
    IndexType nrOfRevisions = revisions.size();
    float changes = 0;
    IdxIdxMap data;

    for (IndexType i = 0; i < nrOfRevisions; i++) {
        IndexType count = m_selectionData->getChangeset()->at(revisions[i]).count();
        changes += count;
        data[count]++;
    }

    ofstream out((m_dataManager->getOutputDir() + "/" + "changes.csv").c_str());
    out << "Number of revisions:;" << nrOfRevisions << endl;
    out << "Number of changes:;" << changes << endl;
    out << "Average changes per revision:; " << changes / nrOfRevisions << endl;
    out << "Number of changes per revision;Occurrences" << endl;
    writeCsv(out, data);
    out.close();
}*/

void CSelectionStatistics::calcFailStatistics(rapidjson::Document &doc)
{
    (cerr << "[INFO] Calculating calcFailStatistics ..." << endl).flush();

    IndexType nrOfRevisions = m_selectionData->getResults()->getNumOfRevisions();
    IntVector revisions = m_selectionData->getResults()->getRevisions().getRevisionNumbers();
    float failed = 0;
    IdxIdxMap data;
    IdxIdxMap revdata;

    for (IndexType i = 0; i < nrOfRevisions; i++) {
        IndexType rev = revisions[i];
        IndexType count = m_selectionData->getResults()->getExecutionBitList(rev).count() - m_selectionData->getResults()->getPassedBitList(rev).count();
        failed += count;
        revdata[revisions[i]] = count;
        data[count]++;
    }

    doc.AddMember("number_of_total_fails", failed, doc.GetAllocator());
    doc.AddMember("average_failed_test_cases_per_revision", failed / nrOfRevisions, doc.GetAllocator());

    rapidjson::Value revFail(rapidjson::kObjectType);
    toJson(revdata, revFail, doc);
    doc.AddMember("revision_fail_histogram", revFail, doc.GetAllocator());

    rapidjson::Value fail(rapidjson::kObjectType);
    toJson(data, fail, doc);
    doc.AddMember("fail_histogram", fail, doc.GetAllocator());
}

void CSelectionStatistics::calcCovResultsSummary(rapidjson::Document &doc)
{
    (cerr << "[INFO] Calculating calcCovResultsSummary ..." << endl).flush();

    IdxIdxMap execData;
    IndexType nOfTestCases = m_selectionData->getCoverage()->getNumOfTestcases();
    IndexType nOfRevisions = m_selectionData->getResults()->getNumOfRevisions();
    IntVector revisions = m_selectionData->getResults()->getRevisionNumbers();

    doc.AddMember("number_of_revisions", nOfRevisions, doc.GetAllocator());
    doc.AddMember("number_of_test_cases", nOfTestCases, doc.GetAllocator());

    rapidjson::Value tcInfos(rapidjson::kObjectType);
    for (IndexType tcid = 0; tcid < nOfTestCases; tcid++) {
        IndexType tcidInResult = m_selectionData->translateTestcaseIdFromCoverageToResults(tcid);
        IndexType execCnt = 0;
        IndexType failedCnt = 0;
        for (IndexType i = 0; i < nOfRevisions; i++) {
            IndexType rev = revisions[i];
            if (m_selectionData->getResults()->getExecutionBitList(rev).at(tcidInResult)) {
                execCnt++;
                if (!m_selectionData->getResults()->getPassedBitList(rev).at(tcidInResult)) {
                    failedCnt++;
                }
            }
        }
        execData[execCnt]++;
        rapidjson::Value tcInfo(rapidjson::kObjectType);
        tcInfo.AddMember("executed", execCnt, doc.GetAllocator());
        tcInfo.AddMember("fail", failedCnt, doc.GetAllocator());
        rapidjson::Value key;
        key.SetString(static_cast<ostringstream*>( &(ostringstream() << tcid) )->str().c_str(), doc.GetAllocator());
        tcInfos.AddMember(key, tcInfo, doc.GetAllocator());
    }

    doc.AddMember("test_case_info", tcInfos, doc.GetAllocator());

    rapidjson::Value exec(rapidjson::kObjectType);
    toJson(execData, exec, doc);
    doc.AddMember("executed_histogram", exec, doc.GetAllocator());

    (cerr << " done" << endl).flush();
}

void CSelectionStatistics::toJson(IdxIdxMap &data, rapidjson::Value &val, rapidjson::Document &root)
{

    for (IdxIdxMap::const_iterator it = data.begin(); it != data.end(); ++it) {
        rapidjson::Value key;
        key.SetString(static_cast<ostringstream*>( &(ostringstream() << it->first) )->str().c_str(), root.GetAllocator());
        rapidjson::Value value;
        value.SetUint64(it->second);
        val.AddMember(key, value, root.GetAllocator());
    }
}

} // namespace soda
