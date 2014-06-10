/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors:
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

#ifndef CREDUCTIONDATA_H
#define CREDUCTIONDATA_H

#include "data/CCoverageMatrix.h"
#include "data/CIDManager.h"

namespace soda {

/**
 * @brief The CReductionData class manages base and reduced coverage data
 *        according to the calls from the reduction plugins.
 */
class CReductionData
{
public:

    CReductionData(CCoverageMatrix* baseCoverage, String programName, String dirPath);
    ~CReductionData();

    /**
     * @brief Adds specified test cases to the reduced coverage matrix.
     * @param testcases Set of specified test cases.
     */
    void add(const std::set<IndexType> &testcases);

    /**
     * @brief Saves the reduced coverage matrix.
     * @param iteration Filename part.
     */
    void save(IndexType iteration);
private:

    /**
     * @brief Test case names.
     */
    CIDManager *m_testcases;

    /**
     * @brief Base coverage data.
     */
    CCoverageMatrix *m_baseCoverage;

    /**
     * @brief Reduced coverage data.
     */
    CCoverageMatrix *m_coverage;

    /**
     * @brief Already inserted test cases.
     */
    std::vector<bool> *m_storedTestcases;

    /**
     * @brief Output file name part.
     */
    String m_programName;

    /**
     * @brief Output directory.
     */
    String m_dirPath;
};

} /* namespace soda */

#endif /* CREDUCTIONDATA_H */
