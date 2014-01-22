/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Havas (havasd@inf.u-szeged.hu)
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
#include "datamanager/CDataHandler.h"

using namespace sodatools;

TEST(CDataHandler, DefaultConstructor)
{
    CDataHandler handler;

    EXPECT_EQ(NULL, handler.getChanges());
    EXPECT_EQ(NULL, handler.getCoverage());
    EXPECT_EQ(NULL, handler.getResults());
    EXPECT_EQ(NULL, handler.getSelection());
    EXPECT_EQ(NULL, handler.getCodeElements());
    EXPECT_EQ(NULL, handler.getTestcases());

    EXPECT_EQ(rfUnknown, handler.getReadFormat());
    EXPECT_TRUE(handler.getPrintInfo());
    EXPECT_FALSE(handler.getWithNames());
    EXPECT_TRUE(handler.getWithPassFail());
}

TEST(CDataHandler, SetAndGet)
{
    CDataHandler handler;

    handler.setWithNames(true);
    EXPECT_TRUE(handler.getWithNames());

    handler.setReadFormat(rfOneTestPerFile);
    EXPECT_EQ(rfOneTestPerFile, handler.getReadFormat());

    EXPECT_TRUE(NULL == handler.getChanges());
    EXPECT_FALSE(NULL == handler.getChanges(true));

    EXPECT_TRUE(NULL == handler.getCoverage());
    EXPECT_FALSE(NULL == handler.getCoverage(true));

    EXPECT_TRUE(NULL == handler.getResults());
    EXPECT_FALSE(NULL == handler.getResults(true));
}
