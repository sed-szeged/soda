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

#include "boost/lexical_cast.hpp"
#include "util/CSelectionStatistics.h"
#include <sstream>
#include <algorithm>
#include <math.h>

using namespace std;

namespace soda {

struct BugStatistic {
    std::set<RevNumType> nrOfFixes;
    std::set<RevNumType> nrOfAffectedCEs;
    time_t fixTime;
};

struct CEStatistic {
    std::set<RevNumType> nrOfFixes;
    std::set<RevNumType> nrOfAffectedBugs;
};


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


    outCodeHistogram( nrOfTestCases, dataCodeElements );
    outTestHistogram( nrOfCodeElements, dataTestCases );


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
    IndexType failed = 0;
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
    doc.AddMember("average_failed_test_cases_per_revision", (float)failed / nrOfRevisions, doc.GetAllocator());

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
    IndexType sumFailedCnt = 0;
    IndexType sumExecCnt = 0;

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
                sumExecCnt++;
                if (!m_selectionData->getResults()->getPassedBitList(rev).at(tcidInResult)) {
                    failedCnt++;
                    sumFailedCnt++;
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

    outTestRunAndExecInfo( execData, sumFailedCnt , sumExecCnt-sumFailedCnt);

    doc.AddMember("test_case_info", tcInfos, doc.GetAllocator());

    rapidjson::Value exec(rapidjson::kObjectType);
    toJson(execData, exec, doc);
    doc.AddMember("executed_histogram", exec, doc.GetAllocator());

    (cerr << " done" << endl).flush();
}

void CSelectionStatistics::calcBugRelatedStatistics(rapidjson::Document &doc)
{
    RevNumType nrOfClosedReports = m_selectionData->getBugs()->getReports().size();
    doc.AddMember("number_of_closed_reports", nrOfClosedReports, doc.GetAllocator());
    calcBugStatisticsForAllBugs(doc);
    calcBugStatisticsForAllCEs(doc);
}

void CSelectionStatistics::calcBugStatisticsForAllBugs(rapidjson::Document &doc)
{

    std::map<RevNumType, BugStatistic> bugStats;
    ReportDataMap const& reportDatas = m_selectionData->getBugs()->getReports();
    for (ReportDataMap::const_iterator it = reportDatas.begin(); it != reportDatas.end(); ++it) {
        bugStats[it->first] = BugStatistic();
        bugStats[it->first].fixTime = it->second.fixTime - it->second.reportTime;
    }

    ReportMap const& reportMap = m_selectionData->getBugs()->getReportMap();
    // revision, <code element, report id>
    for (ReportMap::const_iterator it = reportMap.begin(); it != reportMap.end(); ++it) {
        // code element, report id
        for (CodeElementReports::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            bugStats[it2->second].nrOfAffectedCEs.insert(it2->first);
            bugStats[it2->second].nrOfFixes.insert(it->first);
        }
    }

    rapidjson::Value allBugs(rapidjson::kArrayType);
    std::vector<RevNumType> fixesVec;
    RevNumType sumFixes = 0;
    std::vector<RevNumType> affectedCEVec;
    RevNumType sumAffectedCE = 0;

    for (std::map<RevNumType, BugStatistic>::const_iterator it = bugStats.begin(); it != bugStats.end(); ++it) {
        // collecting data for statistics
        sumFixes += it->second.nrOfFixes.size();
        fixesVec.push_back(it->second.nrOfFixes.size());
        sumAffectedCE += it->second.nrOfAffectedCEs.size();
        affectedCEVec.push_back(it->second.nrOfAffectedCEs.size());

        // creates json array for all reports
        rapidjson::Value bugStat(rapidjson::kObjectType);
        bugStat.AddMember("report_id", it->first, doc.GetAllocator());
        bugStat.AddMember("number_of_fixes", it->second.nrOfFixes.size(), doc.GetAllocator());
        bugStat.AddMember("number_of_affected_ce", it->second.nrOfAffectedCEs.size(), doc.GetAllocator());
        bugStat.AddMember("fix_time", it->second.fixTime, doc.GetAllocator());
        allBugs.PushBack(bugStat, doc.GetAllocator());
    }
    doc.AddMember("per_bug_statistics", allBugs, doc.GetAllocator());

    rapidjson::Value bugStatistics(rapidjson::kObjectType);
    {
        // sorts the numbers for easier statistics calculations
        // number of fixes statistics
        std::sort(fixesVec.begin(), fixesVec.end());
        rapidjson::Value numFixesStat(rapidjson::kObjectType);
        numFixesStat.AddMember("min", fixesVec.front(), doc.GetAllocator());
        numFixesStat.AddMember("max", fixesVec.back(), doc.GetAllocator());
        numFixesStat.AddMember("avg", (float)sumFixes / fixesVec.size(), doc.GetAllocator());
        if (fixesVec.size() % 2 == 0) { // average of the two middle number
            RevNumType nthElem = fixesVec.size() / 2;
            float avg = (fixesVec[nthElem] + fixesVec[nthElem + 1]) / 2;
            numFixesStat.AddMember("med", avg, doc.GetAllocator());
        }
        else { // one middle
            numFixesStat.AddMember("med", fixesVec[ceil(fixesVec.size() / 2)], doc.GetAllocator());
        }
        bugStatistics.AddMember("number_of_fixes", numFixesStat, doc.GetAllocator());
    }

    {
        // affected code elements statistics
        std::sort(affectedCEVec.begin(), affectedCEVec.end());
        rapidjson::Value numAffectedCEStat(rapidjson::kObjectType);
        numAffectedCEStat.AddMember("min", affectedCEVec.front(), doc.GetAllocator());
        numAffectedCEStat.AddMember("max", affectedCEVec.back(), doc.GetAllocator());
        numAffectedCEStat.AddMember("avg", (float)sumAffectedCE / affectedCEVec.size(), doc.GetAllocator());
        if (affectedCEVec.size() % 2 == 0) { // average of the two middle number
            RevNumType nthElem = affectedCEVec.size() / 2;
            float avg = (float)(affectedCEVec[nthElem] + affectedCEVec[nthElem + 1]) / 2;
            numAffectedCEStat.AddMember("med", avg, doc.GetAllocator());
        }
        else { // one middle
            numAffectedCEStat.AddMember("med", affectedCEVec[ceil((float)affectedCEVec.size() / 2)], doc.GetAllocator());
        }
        bugStatistics.AddMember("number_of_affected_ce", numAffectedCEStat, doc.GetAllocator());
    }
    doc.AddMember("bug_statistics", bugStatistics, doc.GetAllocator());

}

void CSelectionStatistics::calcBugStatisticsForAllCEs(rapidjson::Document &doc)
{
    std::map<RevNumType, CEStatistic> ceStats;

    ReportMap const& reportMap = m_selectionData->getBugs()->getReportMap();
    // revision, <code element, report id>
    for (ReportMap::const_iterator it = reportMap.begin(); it != reportMap.end(); ++it) {
        // code element, report id
        for (CodeElementReports::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            if (!ceStats.count(it2->first)) {
                ceStats[it2->first] = CEStatistic();
            }
            ceStats[it2->first].nrOfFixes.insert(it->first);
            ceStats[it2->first].nrOfAffectedBugs.insert(it2->second);
        }
    }

    std::vector<RevNumType> fixesVec;
    RevNumType sumFixes = 0;
    rapidjson::Value allCEs(rapidjson::kArrayType);
    for (std::map<RevNumType, CEStatistic>::const_iterator it = ceStats.begin(); it != ceStats.end(); ++it) {
        // collecting metadata
        fixesVec.push_back(it->second.nrOfFixes.size());
        sumFixes += it->second.nrOfFixes.size();

        // creating json array for all code elements
        rapidjson::Value ceStat(rapidjson::kObjectType);
        rapidjson::Value val(rapidjson::kStringType);
        val.SetString(m_selectionData->getBugs()->getCodeElements().getValue(it->first).c_str(), doc.GetAllocator());
        ceStat.AddMember("ce_name", val, doc.GetAllocator());
        ceStat.AddMember("number_of_fixes", it->second.nrOfFixes.size(), doc.GetAllocator());
        ceStat.AddMember("number_of_affected_reports", it->second.nrOfAffectedBugs.size(), doc.GetAllocator());
        allCEs.PushBack(ceStat, doc.GetAllocator());
    }
    doc.AddMember("per_ce_statistics", allCEs, doc.GetAllocator());

    rapidjson::Value ceStatistics(rapidjson::kObjectType);
    {
        rapidjson::Value numberOfFixesStat(rapidjson::kObjectType);
        // affected code elements statistics
        std::sort(fixesVec.begin(), fixesVec.end());
        rapidjson::Value numAffectedCEStat(rapidjson::kObjectType);
        numberOfFixesStat.AddMember("min", fixesVec.front(), doc.GetAllocator());
        numberOfFixesStat.AddMember("max", fixesVec.back(), doc.GetAllocator());
        numberOfFixesStat.AddMember("avg", (float)sumFixes / fixesVec.size(), doc.GetAllocator());
        if (fixesVec.size() % 2 == 0) { // average of the two middle number
            RevNumType nthElem = fixesVec.size() / 2;
            float avg = (float)(fixesVec[nthElem] + fixesVec[nthElem + 1]) / 2;
            numberOfFixesStat.AddMember("med", avg, doc.GetAllocator());
        }
        else { // one middle
            numberOfFixesStat.AddMember("med", fixesVec[ceil((float)fixesVec.size() / 2)], doc.GetAllocator());
        }
        ceStatistics.AddMember("number_of_fixes", numberOfFixesStat, doc.GetAllocator());
    }
    ceStatistics.AddMember("number_of_faulty_ces", ceStats.size(), doc.GetAllocator());
    doc.AddMember("ce_statistics", ceStatistics, doc.GetAllocator());
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

void CSelectionStatistics::setHistogramParameters(String projectNameString, int sliceSize, int sliceNumber, String outputDir)
{
    m_pojectName = projectNameString;
    m_sliceSize = sliceSize;
    m_sliceNumber = sliceNumber;
    m_outputDir = outputDir;
}

void CSelectionStatistics::outCodeHistogram( int nrOfTestCases, IdxIdxMap dataCodeElements )
{
    std::ofstream outCodeHistogramSize, outCodeHistogramNumber;
    string codeHistogramSizeFileName = m_outputDir+"/"+m_pojectName+"-cov-hist-code-s"+boost::lexical_cast<std::string>(m_sliceSize)+".csv";
    string codeHistogramNumberFileName = m_outputDir+"/"+m_pojectName+"-cov-hist-code-n"+boost::lexical_cast<std::string>(m_sliceNumber)+".csv";

    outCodeHistogramSize.open(codeHistogramSizeFileName.c_str());
    outCodeHistogramSize<<"intervals;count\n";
    for(int i = 0 ; i < nrOfTestCases ; i=i+m_sliceSize){
        int sum = 0;
        for(int j = 0 ; j < m_sliceSize; j++) sum += dataCodeElements[i+j];
        outCodeHistogramSize<<i<<"-"<<i+m_sliceSize-1<<";"<<sum<<"\n";
    }

    std::cout<<nrOfTestCases<<"/"<<m_sliceNumber<<" ---> ceil: "<<ceil(nrOfTestCases/m_sliceNumber)<<std::endl;

    outCodeHistogramNumber.open(codeHistogramNumberFileName.c_str());
    outCodeHistogramNumber<<"intervals;count\n";
    for(int i = 0 ; i < m_sliceNumber  ; i++){
        int sum = 0;
        for(int j = 0 ; j < ceil(nrOfTestCases/m_sliceNumber)+1 ; j++) sum += dataCodeElements[(i*(ceil(nrOfTestCases/m_sliceNumber)+1) )+j];
        outCodeHistogramNumber<< (i*(ceil(nrOfTestCases/m_sliceNumber)+1)) <<"-"<< ((i+1)*(ceil(nrOfTestCases/m_sliceNumber)+1))-1 <<";"<<sum<<"\n";
    }
}



void CSelectionStatistics::outTestHistogram( int nrOfCodeElements, IdxIdxMap dataTestCases )
{
    std::ofstream outTestHistogramSize, outTestHistogramNumber;
    string testHistogramSizeFileName = m_outputDir+"/"+m_pojectName+"-cov-hist-test-s"+boost::lexical_cast<std::string>(m_sliceSize)+".csv";
    string testHistogramNumberFileName = m_outputDir+"/"+m_pojectName+"-cov-hist-test-n"+boost::lexical_cast<std::string>(m_sliceNumber)+".csv";

    outTestHistogramSize.open(testHistogramSizeFileName.c_str());
    outTestHistogramSize<<"intervals;count\n";
    for(int i = 0 ; i < nrOfCodeElements ; i=i+m_sliceSize){
        int sum = 0;
        for(int j = 0 ; j < m_sliceSize; j++) sum += dataTestCases[i+j];
        outTestHistogramSize<<i<<"-"<<i+m_sliceSize-1<<";"<<sum<<"\n";
    }

    outTestHistogramNumber.open(testHistogramNumberFileName.c_str());
    outTestHistogramNumber<<"intervals;count\n";
    for(int i = 0 ; i < m_sliceNumber  ; i++){
        int sum = 0;
        for(int j = 0 ; j < ceil(nrOfCodeElements/m_sliceNumber)+1; j++) sum += dataTestCases[(i*(ceil(nrOfCodeElements/m_sliceNumber)+1) )+j];
        outTestHistogramNumber<< (i*(ceil(nrOfCodeElements/m_sliceNumber)+1)) <<"-"<< ((i+1)*(ceil(nrOfCodeElements/m_sliceNumber)+1))-1 <<";"<<sum<<"\n";
    }
}




void CSelectionStatistics::outTestRunAndExecInfo(IdxIdxMap execData, int fail, int notFail)
{
    int exec=0, notExec=0;
    for( IdxIdxMap::iterator it = execData.begin() ; it != execData.end() ; ++it){
        if( int(it->first) ){
            exec = int(it->second);
        } else {
            notExec = int(it->second);
        }
    }

    std::ofstream outTestInfo;
    string outTestInfoFileName = m_outputDir+"/"+m_pojectName+"-test-run-and-exec-info.csv";
    outTestInfo.open(outTestInfoFileName.c_str());
    outTestInfo<<"test cases;exec. test cases;not exec. test cases;pass test cases;fail test cases\n";
    outTestInfo<<exec+notExec<<";"<<exec<<";"<<notExec<<";"<<notFail<<";"<<fail;

}



} // namespace soda
