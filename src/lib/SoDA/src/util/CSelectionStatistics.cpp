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


rapidjson::Document CSelectionStatistics::calcCoverageRelatedStatistics()
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

    rapidjson::Document covStats;
    covStats.SetObject();
    covStats.AddMember("number_of_code_elements", nrOfCodeElements, covStats.GetAllocator());
    covStats.AddMember("number_of_test_cases", nrOfTestCases, covStats.GetAllocator());
    covStats.AddMember("density", covered / (nrOfTestCases * nrOfCodeElements), covStats.GetAllocator());
    covStats.AddMember("average_test_cases_per_code_elements", covered / nrOfCodeElements, covStats.GetAllocator());
    covStats.AddMember("average_code_elements_per_test_cases", covered / nrOfTestCases, covStats.GetAllocator());
    rapidjson::Value val;
    toJson(dataTestCases, val, covStats);
    covStats.AddMember("test_coverage_histogram", val, covStats.GetAllocator());
    val.Clear();
    toJson(dataCodeElements, val, covStats);
    covStats.AddMember("code_coverage_histogram", val, covStats.GetAllocator());
    return covStats;
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
}

void CSelectionStatistics::calcFailStatistics()
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

    ofstream out((m_dataManager->getOutputDir() + "/" + "fails.csv").c_str());
    out << "Number of revisions:;" << nrOfRevisions << endl;
    out << "Number of fails:;" << failed << endl;
    out << "Revision number;Number of fails" << endl;
    writeCsv(out, revdata);
    out.close();

    out.open((m_dataManager->getOutputDir() + "/" + "fails-statistics.csv").c_str());
    out << "Average failed test cases per revision:; " << failed / nrOfRevisions << endl;
    out << "Number of failed test cases per revision;Occurrences" << endl;
    writeCsv(out, data);
    out.close();
}

void CSelectionStatistics::calcCovResultsSummary()
{
    (cerr << "[INFO] Calculating calcCovResultsSummary ..." << endl).flush();
    String covResultFileName;

    covResultFileName.append(m_dataManager->getOutputDir()).append("/").append("cov-result.csv");
    ofstream output(covResultFileName.c_str());

    const IBitMatrix& coverageMatrix = m_selectionData->getCoverage()->getBitMatrix();
    IndexType nOfTestCases = m_selectionData->getCoverage()->getNumOfTestcases();
    IndexType nOfRevisions = m_selectionData->getResults()->getNumOfRevisions();
    IntVector revisions = m_selectionData->getResults()->getRevisionNumbers();

    output << "Number of revisions:;" << nOfRevisions << endl;
    output << "Number of test cases:;" << nOfTestCases << endl;
    output << "Tcid;Coverage;Executed;Fails" << endl;

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
        output << tcid << ";" << coverageMatrix.getRow(tcid).count() << ";" << execCnt << ";" << failedCnt << endl;
    }

    output.close();
    (cerr << " done" << endl).flush();
}*/

void CSelectionStatistics::toJson(IdxIdxMap &data, rapidjson::Value &val, rapidjson::Document &root)
{
    for (IdxIdxMap::const_iterator it = data.begin(); it != data.end(); ++it) {
        val.AddMember(static_cast<ostringstream*>( &(ostringstream() << it->first) )->str().c_str(), it->second, root.GetAllocator());
    }
}

} // namespace soda
