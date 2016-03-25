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

#ifndef FLINTPRIORITIZATIONPLUGIN_H
#define FLINTPRIORITIZATIONPLUGIN_H

#include "data/CSelectionData.h"
#include "engine/CKernel.h"

namespace soda {

/**
 * @brief Prioritization plugin that is based on coverage information.
 *          - testcase covering more functions has higher coverage
 */
class FlintPrioritizationPlugin : public ITestSuitePrioritizationPlugin
{
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
    FlintPrioritizationPlugin();
    virtual ~FlintPrioritizationPlugin();

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
     * @brief Fills the plugin with data.
     */
    void init(CSelectionData *, CKernel *);

    /**
     * @brief Gets the first n prioritized testcases.
     * @param selected The result vector.
     * @param size The number of testcases we want to select.
     */
    void fillSelection(IntVector& selected, size_t size);

    /**
     * @brief Sets the cinitial state of the algorithm.
     * @param ordered List of already prioritized tests. The algorithm will continue the priorotozation from this point.
     */
    void setState(IntVector& ordered);

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
     * @brief Orders the code elements by their coverage
     */
    void prioritize();

    double tarantula(IndexType tp, IndexType tf, IndexType cp, IndexType cf);

    void calculateCounters();
    void updateCounters(IndexType tcid);
    double entropyLookahead(IndexType tcid);
private:

    CKernel *m_kernel;

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
     * @brief Vector of remaining elements.
     */
    IntVector* m_elementsRemaining;

    /**
     * @brief Priority queue.
     */
    std::vector<qelement>* m_priorityQueue;

    /**
     * @brief Revision
     */
    RevNumType m_revision;

    /**
     * @brief Indicates the state of the algorithm.
     */
    bool m_firstPhase;

    ITestSuitePrioritizationPlugin *m_greedyAlgorithm;

    IntVector m_cpi;
    IntVector m_cfi;

    IndexType m_tpi;
    IndexType m_tfi;

};

} /* namespace soda */

#endif /* FLINTPRIORITIZATIONPLUGIN_H */
