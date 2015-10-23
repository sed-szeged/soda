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

#include <boost/filesystem.hpp>
#include "util/CSelectionStatistics.h"
#include "CDataManager.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"

using namespace std;

namespace soda {

CDataManager::CDataManager() :
    m_selectionData(new CSelectionData()),
    m_testMask(tmNone)
{}

CDataManager::~CDataManager()
{
    if (m_selectionData)
        delete m_selectionData;
}

void CDataManager::loadCoverageData(String path)
{
    m_selectionData->loadCoverage(path);
}

void CDataManager::loadResultsData(String path)
{
    m_selectionData->loadResults(path);
}

void CDataManager::loadChangesetData(String path)
{
    m_selectionData->loadChangeset(path);
}

void CDataManager::calcStatistics()
{
    if(!(boost::filesystem::exists(m_outputDir))) {
        boost::filesystem::create_directories(m_outputDir);
    }

    CSelectionStatistics stats = CSelectionStatistics(m_selectionData);
    if (m_testMask & (tmTestcaseCoverage | tmFunctionCoverage)) {
        rapidjson::Document res;
        res.SetObject();
        stats.calcCoverageRelatedStatistics(res);
        char writeBuffer[65535];
        rapidjson::FileWriteStream f(stdout, writeBuffer, sizeof(writeBuffer));
        rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(f);
        res.Accept(writer);
    }
    /*if (m_testMask & tmFails) {
        stats.calcFailStatistics();
    }*/
    /*if (m_testMask & tmChanges) {
        stats.calcChangeRelatedStatistics();
    }*/
    if (m_testMask & tmCoverageResultSummary) {
        rapidjson::Document res;
        res.SetObject();
        stats.calcCovResultsSummary(res);
        char writeBuffer[65535];
        rapidjson::FileWriteStream f(stdout, writeBuffer, sizeof(writeBuffer));
        rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(f);
        res.Accept(writer);
    }
}

CSelectionData* CDataManager::getSelectionData()
{
    return m_selectionData;
}

int CDataManager::getTestMask()
{
    return m_testMask;
}

void CDataManager::setTestMask(int mask)
{
    m_testMask |= mask;
}

String CDataManager::getOutputDir()
{
    return m_outputDir;
}

void CDataManager::setOutputDir(String path)
{
    m_outputDir = path;
}

} // namespace soda
