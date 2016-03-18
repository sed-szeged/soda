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

#ifndef IPRIORITIZATIONALGORITHMPLUGIN_H
#define IPRIORITIZATIONALGORITHMPLUGIN_H

#include "data/CSelectionData.h"

namespace soda {

/**
 * @brief Interface of the prioritization plugins.
 */
class ITestSuitePrioritizationPlugin
{
public:

    /**
     * @brief Virtual destructor.
     */
    virtual ~ITestSuitePrioritizationPlugin() {}

    /**
     * @brief Returns the name of the plugin.
     * @return
     */
    virtual String getName() = 0;

    /**
     * @brief Returns the description of the plugin.
     * @return
     */
    virtual String getDescription() = 0;

    /**
     * @brief Fills the prioritization plugin with data
     */
    virtual void init(CSelectionData *) = 0;

    /**
     * @brief Sets the cinitial state of the algorithm.
     * @param ordered List of already prioritized tests. The algorithm will continue the priorotozation from this point.
     */
    void setState(IntVector& ordered);

    /**
     * @brief Runs the prioritization on a specified revision.
     */
    virtual void reset(RevNumType) = 0;

    /**
     * @brief Gets the first n prioritized testcases.
     * @param selected The result vector.
     * @param size The number of testcases we want to select.
     */
    virtual void fillSelection(IntVector&, size_t) = 0;
};

} /* namespace soda */

#endif /* IPRIORITIZATIONALGORITHMPLUGIN_H */
