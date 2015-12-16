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
#include "io/CBinaryIO.h"
#include "io/CBitWriter.h"
#include "io/CBitReader.h"

using namespace soda::io;

TEST(CBitWriter, BasicOperations)
{
    CBinaryIO *io = new CBinaryIO("sample/bitio.saved", CBinaryIO::omWrite);
    CBitWriter writer(io);
    EXPECT_NO_THROW(writer.writeBit(true));
    EXPECT_NO_THROW(writer.writeBit(false));
    EXPECT_NO_THROW(writer.writeBit(true));
    EXPECT_NO_THROW(writer.flush());
    EXPECT_NO_THROW(writer.writeBit(false));
    EXPECT_NO_THROW(writer.writeBit(false));
    EXPECT_NO_THROW(writer.writeBit(true));
    EXPECT_NO_THROW(writer.flush());
    EXPECT_NO_THROW(delete io);
}

TEST(CBitWriter, PredictSize)
{
    EXPECT_EQ(10000, CBitWriter::predictSize(200*400));
    EXPECT_EQ(157500000, CBitWriter::predictSize(21000*60000));
}

TEST(CBitReader, BasicOperators)
{
    CBinaryIO *io = new CBinaryIO("sample/bitio.saved", CBinaryIO::omRead);
    CBitReader reader(io);
    EXPECT_EQ(true, reader.readBit());
    EXPECT_EQ(0u, reader.readBit());
    EXPECT_EQ(true, reader.readBit());
    EXPECT_NO_THROW(reader.reset());
    EXPECT_EQ(0u, reader.readBit());
    EXPECT_EQ(0u, reader.readBit());
    EXPECT_EQ(true, reader.readBit());
    EXPECT_NO_THROW(delete io);
}
