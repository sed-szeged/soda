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

#ifndef COVERAGEREDUCTIONPLUGIN_H
#define COVERAGEREDUCTIONPLUGIN_H

#include "data/CSelectionData.h"
#include "engine/CKernel.h"

using namespace std;

namespace soda {

/**
 * @brief The CoverageReductionPlugin class is a reduction plugin which uses coverage for reducing coverage
 *        data.
 */
class CoverageReductionPlugin : public ITestSuiteReductionPlugin
{
public:

    /**
     * @brief Creates a new instance.
     */
    CoverageReductionPlugin();
    virtual ~CoverageReductionPlugin();

    /**
     * @brief Returns the name of the plugin.
     * @return
     */
    String getName();

    /**
     * @brief Returns the description of the plugin.
     * @return
     */
    String getDescription();

    /**
     * @brief Fills the reduction plugin with data.
     */
    void init(CSelectionData *data, CJsonReader &reader);

    /**
     * @brief Runs the reduction plugin.
     * @param outStream Output stream for log file.
     */
    void reduction(std::ofstream &outStream);

private:

    /**
     * @brief Reduces the size of the coverage data.
     * @param outStream Output strema for log file.
     */
    void coverageReduction(std::ofstream &outStream);

    IndexType addCoverage(IBitList& to, const IBitList& from) const;

    bool setSmaller(const IBitList& lhs, const IBitList& rhs) const;

    /**
     * @brief Selection data.
     */
    CSelectionData *m_data;

    /**
     * @brief Output file name.
     */
    String m_programName;

    /**
     * @brief Output directory path.
     */
    String m_dirPath;

    /**
     * @brief Number of code elements.
     */
    IndexType m_nrOfCodeElements;

    /**
     * @brief Number of test cases.
     */
    IndexType m_nrOfTestCases;

    /**
     * @brief Number of covered code elements in a reduction output.
     */
    IndexType coveredCEGoal = 0;
};

} /* namespace soda */

#endif /* COVERAGEREDUCTIONPLUGIN_H */
