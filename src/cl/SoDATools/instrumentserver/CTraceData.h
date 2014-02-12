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

#ifndef CTRACEDATA_H
#define CTRACEDATA_H

#include <pthread.h>
#include "data/CCoverageMatrix.h"

namespace soda {

/**
 * @brief This class holds the trace informations of a program.
 */
class CTraceData
{
public:

    /**
     * @brief Creates a new CTraceData instance.
     * @param coverageFilePath  The output path of the coverage matrix.
     * @param baseDir  The base directory where the executed binaries can be
     *                found.
     */
    CTraceData(const String& coverageFilePath, const String& baseDir);

    /**
      * @brief Destroy a CTraceData instance.
      */
    virtual ~CTraceData();

    /**
     * @brief Saves the coverage information in binary SoDA format.
     */
    virtual void save();

    /**
     * @brief Sets the coverage of the function in the test.
     * @param test  The name of the test.
     * @param functionName  The name of the function.
     */
    virtual void setCoverage(const String &test, const String &functionName);

    /**
     * @brief Returns the name of the function by its address.
     * @param binaryPath  The path to the binary which contains the function.
     * @param address  The address of the function in the binary.
     * @return The name of the function.
     */
    virtual String getFunctionName(const String &binaryPath, const int address);

    /**
     * @brief Adds the function name to the cache to speed up the look up process.
     * @param binaryPath  The path to the binary which contains the function.
     * @param address  The address of the function in the binary.
     * @param functionName  The name of the function.
     */
    virtual void addFunctionName(const String &binaryPath, const int address, const String &functionName);

    /**
     * @brief Returns the base directory where the executed binaries can be
     *        found.
     * @return
     */
    virtual String getBaseDir();

private:
    /**
     * @brief Contains the coverage infomration.
     */
    CCoverageMatrix *m_coverageMatrix;

    /**
     * @brief A mutex to prevent parallel access to the coverage matrix.
     */
    pthread_mutex_t  m_coverageMutex;

    /**
     * @brief The output path of the coverage matrix.
     */
    String           m_coverageFilePath;

    /**
     * @brief Stores the function names and their address to speed up lookup
     *        process.
     */
    std::map<String, std::map<int, String> > m_addressMap;

    /**
     * @brief A mutex to prevent parallel access to the address map.
     */
    pthread_mutex_t  m_addressMapMutex;

    String m_baseDir;
};

} /* namespace soda */

#endif /* CTRACEDATA_H */
