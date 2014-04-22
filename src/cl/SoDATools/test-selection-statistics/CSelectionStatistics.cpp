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

#include "CSelectionStatistics.h"

using namespace std;

namespace soda {

CSelectionStatistics::CSelectionStatistics(CDataManager *mgr) :
    m_dataManager(mgr)
{
}

CSelectionStatistics::~CSelectionStatistics()
{}

CSelectionData* CSelectionStatistics::getSelectionData()
{
    return m_dataManager->getSelectionData();
}

void CSelectionStatistics::calcCoverageRelatedStatistics()
{
    (cerr << "[INFO] Calculating coverage statistics ..." << endl).flush();

    IndexType nrOfCodeElements = getSelectionData()->getCoverage()->getNumOfCodeElements();
    IndexType nrOfTestCases = getSelectionData()->getCoverage()->getNumOfTestcases();
    IdxIdxMap dataCodeElements;
    IdxIdxMap dataTestCases;
    IntVector coveredCodeElements(nrOfCodeElements);
    float covered = 0;

    for (IndexType tcid = 0; tcid < nrOfTestCases; tcid++) {
        (cerr << "\r[INFO] Calculating coverage statistics ... " << (tcid + 1) << "/" << nrOfTestCases).flush();
        IndexType count = 0;
        for (IndexType ceid = 0; ceid < nrOfCodeElements; ceid++) {
            if (getSelectionData()->getCoverage()->getBitMatrix().get(tcid, ceid)) {
                coveredCodeElements[ceid]++;
                count++;
            }
        }
        covered += count;
        dataTestCases[count]++;
    }

    (cerr << endl).flush();

    if (m_dataManager->getTestMask() & tmTestcaseCoverage) {
        ofstream out((m_dataManager->getOutputDir() + "/" + "testcaseCoverage.csv").c_str());
        out << "Number of code elements:;" << nrOfCodeElements << endl;
        out << "Number of test cases:;" << nrOfTestCases << endl;
        out << "Density of coverage matrix: " << covered / (nrOfTestCases * nrOfCodeElements) << endl;
        out << "Average code elements per testcases: " << covered / nrOfTestCases << endl;
        out << "Number of covered code elements per test case;Occurrences" << endl;
        writeCsv(out, dataTestCases);
        out.close();
    }

    if (m_dataManager->getTestMask() & tmFunctionCoverage) {
        for (IndexType ceid = 0; ceid < nrOfCodeElements; ceid++) {
            dataCodeElements[coveredCodeElements[ceid]]++;
        }

        ofstream out((m_dataManager->getOutputDir() + "/" + "codeElementCoverage.csv").c_str());
        out << "Number of code elements:;" << nrOfCodeElements << endl;
        out << "Number of test cases:;" << nrOfTestCases << endl;
        out << "Density of coverage matrix:; " << covered / (nrOfTestCases * nrOfCodeElements) << endl;
        out << "Average testcases per code elements:; " << covered / nrOfCodeElements << endl;
        out << "Number of test cases per code element;Occurrences" << endl;
        writeCsv(out, dataCodeElements);
        out.close();
    }
}

void CSelectionStatistics::calcChangeRelatedStatistics()
{
    (cerr << "[INFO] Calculating calcChangeRelatedStatistics ..." << endl).flush();

    const IntVector revisions = getSelectionData()->getChangeset()->getRevisions();
    IndexType nrOfRevisions = revisions.size();
    float changes = 0;
    IdxIdxMap data;

    for (IndexType i = 0; i < nrOfRevisions; i++) {
        IndexType count = getSelectionData()->getChangeset()->at(revisions[i]).count();
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

    IndexType nrOfRevisions = getSelectionData()->getResults()->getNumOfRevisions();
    IntVector revisions = getSelectionData()->getResults()->getRevisions().getRevisionNumbers();
    float failed = 0;
    IdxIdxMap data;
    IdxIdxMap revdata;

    for (IndexType i = 0; i < nrOfRevisions; i++) {
        IndexType rev = revisions[i];
        IndexType count = getSelectionData()->getResults()->getExecutionBitList(rev).count() - getSelectionData()->getResults()->getPassedBitList(rev).count();
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

    const IBitMatrix& coverageMatrix = getSelectionData()->getCoverage()->getBitMatrix();
    IndexType nOfTestCases = getSelectionData()->getCoverage()->getNumOfTestcases();
    IndexType nOfRevisions = getSelectionData()->getResults()->getNumOfRevisions();
    IntVector revisions = getSelectionData()->getResults()->getRevisionNumbers();

    output << "Number of revisions:;" << nOfRevisions << endl;
    output << "Number of test cases:;" << nOfTestCases << endl;
    output << "Tcid;Coverage;Executed;Fails" << endl;

    for (IndexType tcid = 0; tcid < nOfTestCases; tcid++) {
        IndexType tcidInResult = getSelectionData()->translateTestcaseIdFromCoverageToResults(tcid);
        IndexType execCnt = 0;
        IndexType failedCnt = 0;
        for (IndexType i = 0; i < nOfRevisions; i++) {
            IndexType rev = revisions[i];
            if (getSelectionData()->getResults()->getExecutionBitList(rev).at(tcidInResult)) {
                execCnt++;
                if (!getSelectionData()->getResults()->getPassedBitList(rev).at(tcidInResult)) {
                    failedCnt++;
                }
            }
        }
        output << tcid << ";" << coverageMatrix.getRow(tcid).count() << ";" << execCnt << ";" << failedCnt << endl;
    }

    output.close();
    (cerr << " done" << endl).flush();
}

void CSelectionStatistics::writeCsv(ofstream& out, IdxIdxMap data)
{
    for (IdxIdxMap::iterator iterator = data.begin(); iterator != data.end(); iterator++) {
        out << iterator->first << ";" << iterator->second << endl;
    }
}

} // namespace soda
