/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Tengeri <dtengeri@inf.u-szeged.hu>
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

#include "data/CTraceData.h"
#include "exception/CException.h"

namespace soda {

CTraceData::CTraceData(const String& coverageFileName, const String& baseDir ) :
    m_coverageMatrix(new CCoverageMatrix()),
    m_coverageFilePath(coverageFileName),
    m_codeElementLocations(new std::set<std::string>()),
    m_baseDir(baseDir)
{
    pthread_mutex_init(&m_coverageMutex, NULL);
    pthread_mutex_init(&m_addressMapMutex, NULL);
    pthread_mutex_init(&m_codeElementLocationsMutex, NULL);
}

CTraceData::~CTraceData()
{
    delete m_coverageMatrix;
    delete m_codeElementLocations;
    pthread_mutex_destroy(&m_coverageMutex);
    pthread_mutex_destroy(&m_addressMapMutex);
    pthread_mutex_destroy(&m_codeElementLocationsMutex);
}

void CTraceData::save()
{
    pthread_mutex_lock(&m_coverageMutex);
    m_coverageMatrix->save(m_coverageFilePath);
    pthread_mutex_unlock(&m_coverageMutex);
}

void CTraceData::setCoverage(const String &test, const String &codeElementName)
{
    pthread_mutex_lock(&m_coverageMutex);
    m_coverageMatrix->addOrSetRelation(test, codeElementName, true);
    pthread_mutex_unlock(&m_coverageMutex);
}

bool CTraceData::getCodeElementName(const String &binaryPath, const int address, String &function)
{
    pthread_mutex_lock(&m_addressMapMutex);
    if (m_addressMap.find(binaryPath) == m_addressMap.end()) {
        pthread_mutex_unlock(&m_addressMapMutex);
        return false;
    }
    if (m_addressMap[binaryPath].find(address) == m_addressMap[binaryPath].end()) {
        pthread_mutex_unlock(&m_addressMapMutex);
        return false;
    }
    function = m_addressMap[binaryPath][address];
    pthread_mutex_unlock(&m_addressMapMutex);
    return true;
}

void CTraceData::addCodeElementName(const String &binaryPath, const int address, const String &codeElementName)
{
    pthread_mutex_lock(&m_addressMapMutex);
    m_addressMap[binaryPath][address] = codeElementName;
    pthread_mutex_unlock(&m_addressMapMutex);
}

void CTraceData::addCodeElementLocation(const String &location)
{
    pthread_mutex_lock(&m_codeElementLocationsMutex);
    m_codeElementLocations->insert(location);
    pthread_mutex_unlock(&m_codeElementLocationsMutex);
}

std::set<std::string>& CTraceData::getCodeElementLocations()
{
    return *m_codeElementLocations;
}

String CTraceData::getBaseDir()
{
    return m_baseDir;
}

} /* namespace soda */
