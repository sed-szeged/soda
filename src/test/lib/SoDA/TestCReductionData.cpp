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

#include "gtest/gtest.h"
#include "data/CReductionData.h"
#include <iostream>
using namespace soda;

TEST(CReductionData, BasicOperations)
{
    CCoverageMatrix base;
    EXPECT_NO_THROW(base.load("sample/MetricPluginSampleDir/SoDA.extended.cov.SoDA"));
    CReductionData data(&base, "SoDA", "sample/");
    std::set<IndexType> tests = { 1, 10, 20, 50 };
    EXPECT_NO_THROW(data.add(tests));
    EXPECT_NO_THROW(data.save(0));
}

TEST(CReductionData, Validate)
{
    CCoverageMatrix base;
    EXPECT_NO_THROW(base.load("sample/SoDA.cov.000.SoDA"));
    EXPECT_EQ(4u, base.getTestcases().size());
    EXPECT_EQ("CBitList.Iterators", base.getTestcases().getValue(1));
    EXPECT_EQ("CBitMatrix.Extend", base.getTestcases().getValue(2));
}
