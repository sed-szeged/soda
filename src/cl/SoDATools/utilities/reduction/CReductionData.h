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

#ifndef CFLREDUCTIONDATA_H
#define CFLREDUCTIONDATA_H

#include "data/CCoverageMatrix.h"
#include "data/CIDManager.h"

namespace soda {

class CReductionData
{
public:
    CReductionData(CCoverageMatrix* baseCoverage, String programName, String dirPath);
    ~CReductionData();

    void add(const std::set<IndexType> &testcases);
    void save(IndexType iteration);
private:
    CIDManager      *m_testcases;

    CCoverageMatrix *m_baseCoverage;
    CCoverageMatrix *m_coverage;

    std::vector<bool> *m_storedTestcases;

    String m_programName;
    String m_dirPath;
};

} /* namespace soda */

#endif /* CFLREDUCTIONDATA_H */
