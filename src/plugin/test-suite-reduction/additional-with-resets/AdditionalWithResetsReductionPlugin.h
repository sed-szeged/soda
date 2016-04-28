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

#ifndef ADDITIONALWITHRESETSREDUCTIONPLUGIN_H
#define ADDITIONALWITHRESETSREDUCTIONPLUGIN_H

#include "data/CSelectionData.h"
#include "engine/CKernel.h"

namespace soda {

/**
 * @brief The AdditionalWithResetsReductionPlugin class is a reduction plugin which is based on
 *        the coverage of code elements not yet covered. The priority information gets reinitalized
 *        every time the algorithm cannot decide the next testcase (i.e. highest priority is zero).
 */
class AdditionalWithResetsReductionPlugin : public ITestSuiteReductionPlugin
{
private:
    typedef struct {
        IndexType testcaseId;
        IndexType priorityValue;
    } qelement;
    friend bool operator<(qelement d1, qelement d2);

public:

    /**
     * @brief Creates a new object.
     */
    AdditionalWithResetsReductionPlugin();

    /**
     * @brief Deinitializes an instance.
     */
    virtual ~AdditionalWithResetsReductionPlugin();

    /**
     * @brief Returns the name of the plugin.
     * @return "additional-with-resets"
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
     */
    void reduction(std::ofstream&);

private:

    /**
     * @brief Reduces the size of the coverage data.
     */
    void additionalWithResetsReduction(std::ofstream&);

    /**
     * @brief Updates the priority queue where necessary.
     * @param testcaseId The identifier of the last selected testcase.
     */
    void update(IndexType testcaseId);

    /**
     * @brief (Re)sets the state of the algorithm based on the given testcases.
     * @param testcases A set of the identifiers of the already available testcases.
     * @param clearAll Whether to clear the set of all covered code elements.
     */
    void setState(const std::set<IndexType>& testcases, bool clearAll = false);

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

    /**
     * @brief Priority queue.
     */
    std::vector<qelement>* m_priorityQueue;

    /**
     * @brief List of not covered code element ids.
     */
    std::list<IndexType>* m_notCoveredCEIDs;

    /**
     * @brief Set of all covered code element ids.
     */
    std::set<IndexType>* m_allCoveredCEIDs;
};

} /* namespace soda */

#endif /* ADDITIONALWITHRESETSREDUCTIONPLUGIN_H */
