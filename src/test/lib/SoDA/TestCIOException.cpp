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
#include "exception/CIOException.h"

using namespace soda;
using namespace soda::io;

TEST(CIOException, ThrowAMessage)
{
    CIOException e("CException::ThrowAMessage", "Test message");
    CIOException *e2 = new CIOException("CException::ThrowAMessage", "Test message");

    EXPECT_EQ(String("soda::IOException@CException::ThrowAMessage: Test message"), String(e2->what()));
    EXPECT_EQ(String("soda::IOException@CException::ThrowAMessage: Test message"), String(e.what()));
    EXPECT_THROW(throw e, CIOException);

    delete e2;
}
