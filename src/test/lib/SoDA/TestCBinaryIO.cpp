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
#include "exception/CIOException.h"

using namespace soda;
using namespace soda::io;

class CBinaryIOTest : public testing::Test
{
protected:
    CBinaryIO *io;

    virtual void SetUp() {
        io = new CBinaryIO();
    }

    virtual void TearDown() {
        delete io;
        io = 0;
    }
};

TEST_F(CBinaryIOTest, BasicOperations)
{
    EXPECT_FALSE(io->isOpen());
    EXPECT_THROW(io->close(), CIOException);
    EXPECT_THROW(io->open(String("sample/notexists"), CBinaryIO::omRead), CIOException);
    EXPECT_NO_THROW(io->open(String("sample/CoverageMatrixSampleBit"), CBinaryIO::omRead));
    EXPECT_TRUE(io->isOpen());
    EXPECT_FALSE(io->eof());
    EXPECT_NO_THROW(io->close());
}

TEST_F(CBinaryIOTest, InvalidReadOperations)
{
    EXPECT_THROW(io->readBool1(), CIOException);

    EXPECT_NO_THROW(io->open("sample/binaryIOTest.saved", CBinaryIO::omWrite));

    EXPECT_THROW(io->readBool1(), CIOException);
    EXPECT_THROW(io->readByte1(), CIOException);
    IndexType rev;
    EXPECT_THROW(io->readData(&rev, sizeof(IndexType)), CIOException);
    EXPECT_THROW(io->readInt4(), CIOException);
    EXPECT_THROW(io->readLongLong8(), CIOException);
    EXPECT_THROW(io->readString(), CIOException);
    EXPECT_THROW(io->readUByte1(), CIOException);
    EXPECT_THROW(io->readUInt4(), CIOException);
    EXPECT_THROW(io->readULongLong8(), CIOException);
}

TEST_F(CBinaryIOTest, WriteOperations)
{
    EXPECT_NO_THROW(io->open("sample/binaryIOTest.saved", CBinaryIO::omWrite));

    EXPECT_NO_THROW(io->writeBool1(true));
    EXPECT_NO_THROW(io->writeByte1(50));
    IndexType rev = 12345678;
    EXPECT_NO_THROW(io->writeData(&rev, sizeof(IndexType)));
    EXPECT_NO_THROW(io->writeInt4(62345));
    EXPECT_NO_THROW(io->writeLongLong8(83242352));
    EXPECT_NO_THROW(io->writeString(String("testString")));
    EXPECT_NO_THROW(io->writeUByte1(23));
    EXPECT_NO_THROW(io->writeUInt4(87235));
    EXPECT_NO_THROW(io->writeULongLong8(83249832));
}

TEST_F(CBinaryIOTest, InvalidWriteOperations)
{
    EXPECT_THROW(io->writeBool1(true), CIOException);

    EXPECT_NO_THROW(io->open("sample/binaryIOTest.saved", CBinaryIO::omRead));

    EXPECT_THROW(io->writeBool1(true), CIOException);
    EXPECT_THROW(io->writeByte1(50), CIOException);
    IndexType rev = 12345678;
    EXPECT_THROW(io->writeData(&rev, sizeof(IndexType)), CIOException);
    EXPECT_THROW(io->writeInt4(62345), CIOException);
    EXPECT_THROW(io->writeLongLong8(83242352), CIOException);
    EXPECT_THROW(io->writeString(String("testString")), CIOException);
    EXPECT_THROW(io->writeUByte1(23), CIOException);
    EXPECT_THROW(io->writeUInt4(87235), CIOException);
    EXPECT_THROW(io->writeULongLong8(83249832), CIOException);
}

TEST_F(CBinaryIOTest, ReadOperations)
{
    EXPECT_NO_THROW(io->open("sample/binaryIOTest.saved", CBinaryIO::omRead));

    EXPECT_NO_THROW(EXPECT_TRUE(io->readBool1()));
    EXPECT_NO_THROW(EXPECT_EQ(50, io->readByte1()));
    IndexType rev;
    EXPECT_NO_THROW(io->readData(&rev, sizeof(IndexType)));
    EXPECT_EQ(12345678u, rev);
    EXPECT_NO_THROW(EXPECT_EQ(62345, io->readInt4()));
    EXPECT_NO_THROW(EXPECT_EQ(83242352, io->readLongLong8()));
    EXPECT_NO_THROW(EXPECT_EQ("testString", io->readString()));
    EXPECT_NO_THROW(EXPECT_EQ(23u, io->readUByte1()));
    EXPECT_NO_THROW(EXPECT_EQ(87235u, io->readUInt4()));
    EXPECT_NO_THROW(EXPECT_EQ(83249832u, io->readULongLong8()));
}
