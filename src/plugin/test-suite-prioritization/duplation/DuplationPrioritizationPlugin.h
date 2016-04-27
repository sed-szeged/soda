/*
 * Copyright (C): 2013-2016 Department of Software Engineering, University of Szeged
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


#ifndef DUPLATIONPRIORITIZATIONPLUGIN_H
#define DUPLATIONPRIORITIZATIONPLUGIN_H

#include "data/CSelectionData.h"
#include "engine/CKernel.h"

namespace soda {

/**
 * @brief Prioritization plugin which optimize for fault localization.
 */
class DuplationPrioritizationPlugin : public ITestSuitePrioritizationPlugin {
public:

    /**
     * @brief Creates a new instance.
     */
    DuplationPrioritizationPlugin();
    virtual ~DuplationPrioritizationPlugin();

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

    void updatePartitions(std::map<IndexType, IndexType> &selectedTestcases);
private:

    /**
     * @brief Selection data.
     */
    CSelectionData*        m_data;

    /**
     * @brief Number of ready elements.
     */
    size_t                 m_nofElementsReady;

    /**
     * @brief Vector of ready elements.
     */
    IntVector*             m_elementsReady;

    /**
     * @brief Vector of remaining elements.
     */
    IntVector* m_elementsRemaining;

    /**
     * @brief The current partition id of code elements
     */
    std::vector<IndexType> *m_partitions;
    std::map<IndexType, IndexType> *m_partitionSizes;
    IndexType m_nextTcid;


};

} /* namespace soda */

#endif /* DUPLATIONPRIORITIZATIONPLUGIN_H */
