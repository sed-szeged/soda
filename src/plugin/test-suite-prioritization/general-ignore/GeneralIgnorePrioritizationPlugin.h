/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: Tamás Gergely <gertom@inf.u-szeged.hu>
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

#ifndef GENERALIGNOREPRIORITIZATIONPLUGIN_H
#define GENERALIGNOREPRIORITIZATIONPLUGIN_H

#include "data/CSelectionData.h"
#include "engine/CKernel.h"

namespace soda {

/**
 * @brief Prioritization plugin that is based on coverage information.
 *          - testcase covering more functions has higher coverage
 */
class GeneralIgnorePrioritizationPlugin : public ITestSuitePrioritizationPlugin
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
    GeneralIgnorePrioritizationPlugin();
    virtual ~GeneralIgnorePrioritizationPlugin();

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
    void init(CSelectionData *);

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

private:
    /**
     * @brief Orders the code elements by their coverage
     */
    void prioritize();

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

};

} /* namespace soda */

#endif /* GENERALIGNOREPRIORITIZATIONPLUGIN_H */
