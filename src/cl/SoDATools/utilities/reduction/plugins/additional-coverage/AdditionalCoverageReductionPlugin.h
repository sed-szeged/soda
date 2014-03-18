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

#ifndef ADDITIONALCOVERAGEREDUCTIONPLUGIN_H
#define ADDITIONALCOVERAGEREDUCTIONPLUGIN_H

#include "data/CSelectionData.h"
#include "../IReductionPlugin.h"

namespace soda {

class AdditionalCoverageReductionPlugin : public IReductionPlugin
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
    AdditionalCoverageReductionPlugin();
    ~AdditionalCoverageReductionPlugin();

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
    void init(CSelectionData *data, String programName, String dirPath, unsigned int iterationLimit = 15);
    void reduction(std::ofstream&);

private:

    void additionalCoverageReduction(std::ofstream&);

    CSelectionData *m_data;

    String m_programName;
    String m_dirPath;

    IndexType m_nrOfCodeElements;
    IndexType m_nrOfTestCases;
};

} /* namespace soda */

#endif /* ADDITIONALCOVERAGEREDUCTIONPLUGIN_H */
