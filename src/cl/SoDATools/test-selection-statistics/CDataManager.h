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

#ifndef CDATAMANAGER_H
#define CDATAMANAGER_H

#include "data/CSelectionData.h"

namespace soda {

enum TestMask
{
    tmNone = 0x00,
    tmTestcaseCoverage = 0x01,
    tmFunctionCoverage = 0x02,
    tmChanges = 0x04,
    tmFails = 0x08,
    tmCoverageResultSummary = 0x10
};

/**
 * @brief The CDataManager class stores selection data and parameters from test selection statistics.
 */
class CDataManager
{
public:

    /**
     * @brief Constructor, creates an empty CDataHandler object.
     */
    CDataManager();

    /**
     * @brief Destroys a CDataHandler object.
     */
    ~CDataManager();

    /**
     * @brief Loads coverage data from the specified path.
     * @param path Path to coverage data.
     */
    void loadCoverageData(String path);

    /**
     * @brief Loads results data from the specified path.
     * @param path Path to results data.
     */
    void loadResultsData(String path);

    /**
     * @brief Loads changeset data from the specified path.
     * @param path Path to changeset data.
     */
    void loadChangesetData(String path);

    /**
     * @brief Calculates selection statistics according to test mask.
     */
    void calcStatistics();

    /**
     * @brief Returns a pointer to m_selectionData.
     * @return A pointer to m_selectionData.
     */
    CSelectionData* getSelectionData();

    /**
     * @brief Returns the value of m_outputDir attribute.
     * @return Value of m_outputDir.
     */
    String getOutputDir();

    /**
     * @brief Sets the specified value to m_outputDir.
     * @param path Output directory path.
     */
    void setOutputDir(String path);

    /**
     * @brief Returns the value of m_testMask.
     * @return  Value of m_testMask.
     */
    int getTestMask();

    /**
     * @brief Sets the specified test masks to m_testMask.
     * @param mask Test mask.
     */
    void setTestMask(int mask);

private:

    /**
     * @brief Stores selection data.
     */
    CSelectionData *m_selectionData;

    /**
     * @brief Output directory path.
     */
    String m_outputDir;

    /**
     * @brief Test mask stores the requested statistic methods.
     */
    int m_testMask;
};

} // namespace soda

#endif /* CDATAMANAGER_H */
