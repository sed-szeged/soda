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

#ifndef CCLUSTERDEFINITION_H
#define CCLUSTERDEFINITION_H

#include <vector>
#include "data/SoDALibDefs.h"

namespace soda {

/**
 * @brief Represents a cluster of test cases and methods.
 */
class CClusterDefinition
{
public:
    CClusterDefinition();
    CClusterDefinition(const CClusterDefinition &other);
    ~CClusterDefinition();

    CClusterDefinition& operator=(const CClusterDefinition &rhs);

    void addTestCase(IndexType tcid);
    void addTestCases(const std::vector<IndexType> &testCases);
    void addCodeElement(IndexType cid);
    void addCodeElements(const std::vector<IndexType> &codeElements);

    const std::vector<IndexType>& getTestCases();
    const std::vector<IndexType>& getCodeElements();
private:
    std::vector<IndexType> *m_testCases;
    std::vector<IndexType> *m_codeElements;
};

} /* namespace soda */

#endif /* CCLUSTERDEFINITION_H */
