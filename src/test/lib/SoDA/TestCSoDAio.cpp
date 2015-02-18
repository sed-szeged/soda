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
#include "io/CSoDAio.h"
#include "exception/CIOException.h"

using namespace soda;
using namespace soda::io;

TEST(CSoDAio, Open)
{
    CSoDAio io;
    EXPECT_NO_THROW(io.open(String("sample/ResultsMatrixSampleBit"), io::CBinaryIO::omRead));
    EXPECT_NO_THROW(io.open("sample/ResultsMatrixSampleBit", io::CBinaryIO::omRead));
    EXPECT_THROW(io.open("sample/ChangesetOneTestPerFileSampleDir/123456", CBinaryIO::omRead), CIOException);
    EXPECT_THROW(io.open("sample/ResultsMatrixSampleBit", CBinaryIO::eOpenMode(4)), CIOException);
}

TEST(CSoDAio, ChunkOperations)
{
    CSoDAio *io = new CSoDAio(String("sample/ioTest"), CBinaryIO::omWrite);

    EXPECT_THROW(io->findChunkID(CSoDAio::EXECUTION), CIOException);
    EXPECT_NO_THROW(io->close());

    EXPECT_THROW(io->findChunkID(CSoDAio::EXECUTION), CIOException);

    EXPECT_NO_THROW(io->open("sample/CoverageMatrixSampleBit", CBinaryIO::omRead));
    EXPECT_EQ(CSoDAio::UNKNOWN_TYPE, io->getChunkID());
    EXPECT_TRUE(io->findChunkID(CSoDAio::TCLIST));
    EXPECT_EQ(CSoDAio::TCLIST, io->getChunkID());

    EXPECT_TRUE(io->findChunkID(CSoDAio::PRLIST));
    EXPECT_TRUE(io->findChunkID(CSoDAio::COVERAGE));

    EXPECT_FALSE(io->findChunkID(CSoDAio::RELATION));
    delete io;
}

TEST(CSoDAio, GetActualLength)
{
    CSoDAio *io = new CSoDAio(String("sample/CoverageMatrixSampleBit"), CBinaryIO::omRead);
    EXPECT_TRUE(io->findChunkID(CSoDAio::TCLIST));
    EXPECT_EQ(182u, io->getActualLength());
    delete io;
}
