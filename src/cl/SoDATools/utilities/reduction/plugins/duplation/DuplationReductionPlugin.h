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

#ifndef DUPLATIONREDUCTIONPLUGIN_H
#define DUPLATIONREDUCTIONPLUGIN_H

#include "data/CSelectionData.h"
#include "../IReductionPlugin.h"

namespace soda {

class DuplationReductionPlugin : public IReductionPlugin
{
public:
    DuplationReductionPlugin();
    ~DuplationReductionPlugin();
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
    void reduction(std::ofstream &outStream);

private:

    void duplationReduction(std::ofstream &outStream);

    void shuffle(std::vector<IndexType>& T);
    int addRandom(std::vector<IndexType>& from, std::set<IndexType>& to, unsigned int N);

    CSelectionData *m_data;

    String m_programName;
    String m_dirPath;

    IndexType m_nrOfCodeElements;
    IndexType m_nrOfTestCases;

    unsigned int m_iterationLimit;
};

} /* namespace soda */

#endif /* DUPLATIONREDUCTIONPLUGIN_H */
