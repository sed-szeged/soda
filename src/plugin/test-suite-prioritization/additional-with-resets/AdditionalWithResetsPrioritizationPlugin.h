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

#ifndef ADDITIONALWITHRESETSPRIORITIZATIONPLUGIN_H
#define ADDITIONALWITHRESETSPRIORITIZATIONPLUGIN_H

#include "data/CSelectionData.h"
#include "engine/CKernel.h"

namespace soda {

/**
 *  @brief Prioritization is based on coverage information. Testcase covering more not yet covered
 *         functions has higher coverage. The priority information gets reinitalized every time the
 *         algorithm cannot decide the next testcase (i.e. highest priority is zero).
 */
class AdditionalWithResetsPrioritizationPlugin : public ITestSuitePrioritizationPlugin {
private:
    typedef struct {
        IndexType testcaseId;
        IndexType priorityValue;
    } qelement;
    friend bool operator<(qelement d1, qelement d2);

public:

    /**
     * @brief Creates a new instance.
     */
    AdditionalWithResetsPrioritizationPlugin();

    /**
     * @brief Deinitializes an instance.
     */
    virtual ~AdditionalWithResetsPrioritizationPlugin();

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
     * @brief Fills the plugin with data.
     */
    void init(CSelectionData *, CKernel *);

    /**
     * @brief Gets the first n prioritized testcases.
     * @param selected A vector that will be used to store the ids of prioritized testcases.
     * @param size The number of testcases we want to select.
     */
    void fillSelection(IntVector& selected, size_t size);

    /**
     * @brief Sets the initial state of the algorithm.
     * @param ordered List of already prioritized tests. The algorithm will continue the prioritization from this point.
     */
    void setState(IntVector& ordered);

    /**
     * @brief Sets the initial state of the algorithm.
     * @param ordered List of already prioritized tests. The algorithm will continue the prioritization from this point.
     * @param update This variable tells the algorithm whether to update the priority queue based on the coverage of already prioritized tests.
     */
    void setState(IntVector& ordered, bool update);


    /**
     * @brief Revision data is not used by this plugin.
     */
    void reset(RevNumType);

    /**
     * @brief Returns the next testcase id in the prioritized order.
     * @return
     */
    IndexType next();

private:
    /**
     * @brief Updates the internal state based on the coverage of the given test.
     * @param tcid The id of the test
     */
    void updateData(IndexType tcid);
private:

    /**
     * @brief Selection data.
     */
    CSelectionData* m_data;

    /**
     * @brief Number of ready elements.
     */
    size_t m_nofElementsReady;

    /**
     * @brief Vector of ready elements.
     */
    IntVector* m_elementsReady;

    /**
     * @brief Priority queue.
     */
    std::vector<qelement>* m_priorityQueue;

    /**
     * @brief List of not covered code element ids.
     */
    std::list<IndexType>* m_notCoveredCEIDs;

    /**
     * @brief Indicates whether a reinitialization should be performed.
     */
    bool m_performReset;
};

} /* namespace soda */

#endif /* ADDITIONALWITHRESETSPRIORITIZATIONPLUGIN_H */
