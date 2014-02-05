/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: László Langó <lango@inf.u-szeged.hu>
 *          Tamás Gergely <gertom@inf.u-szeged.hu>
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

#include <stdexcept>
#include "gtest/gtest.h"
#include "data/CRevision.h"

using namespace soda;

TEST(CRevisionTest, BasicOperations)
{
    CRevision<String> stringRevisions;

    IndexType keys[7] = {1u,  5u,  10u, 50u, 100u, 500u, 1000u};
    String    vals[7] = {"I", "V", "X", "L", "C",  "D",  "M"};
    for (int i = 0; i < 7; i++) {
        stringRevisions.addRevision(keys[i], vals[i]);
        EXPECT_EQ(stringRevisions.getRevision(keys[i]), vals[i]);
        EXPECT_EQ(stringRevisions.size(), (IndexType)i+1);
    }

    IntVector ids = stringRevisions.getRevisionNumbers();
    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(ids[i], keys[i]);
    }

    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(stringRevisions[keys[i]], vals[i]);
        stringRevisions.removeRevision(keys[i]);
        EXPECT_THROW(stringRevisions.getRevision(keys[i]), std::out_of_range);
        EXPECT_EQ(stringRevisions.size(), (IndexType)6-i);
    }
}

TEST(CRevision, SaveAndLoad)
{
    IndexType keys[7] = {1u,  5u,  10u, 50u, 100u, 500u, 1000u};
    int vals[7] = {50, 70, 20, 110, 5221, 67000, 213123 };

    CRevision<int>* intRevisions = new CRevision<int>();
    for (int i = 0; i < 7; ++i)
        intRevisions->addRevision(keys[i], vals[i]);

    io::CSoDAio *io = new io::CSoDAio("sample/revisionTest.saved", io::CBinaryIO::omWrite);
    EXPECT_NO_THROW(intRevisions->save(io));
    delete io;

    CRevision<int>* intRevisions2 = new CRevision<int>();
    io = new io::CSoDAio("sample/revisionTest.saved", io::CBinaryIO::omRead);
    EXPECT_TRUE(io->findChunkID(io::CSoDAio::REVISIONS));
    EXPECT_NO_THROW(intRevisions2->load(io));

    EXPECT_EQ(intRevisions->getRevisionNumbers().size(), intRevisions2->getRevisionNumbers().size());
    for (int i = 0; i < 7; ++i) {
        EXPECT_EQ(intRevisions->getRevision(keys[i]), intRevisions2->getRevision(keys[i]));
    }

    delete intRevisions;
    delete intRevisions2;
}
