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

#include "gtest/gtest.h"
#include "datareader/CReadChangeset.h"
#include "data/CChangeset.h"

using namespace soda;
using namespace sodatools;

class CReadChangesetTest : public testing::Test
{
protected:
    CChangeset* changeset;
    CReadChangeset* reader;

    virtual void SetUp() {
        changeset = new CChangeset();
        reader    = new CReadChangeset(changeset, true, false);
    }

    virtual void TearDown() {
        delete reader;
        reader = 0;
        delete changeset;
        changeset = 0;
    }

};

TEST_F(CReadChangesetTest, readOneTestPerFileChangesetFromDirectoryStructure)
{
    reader->readOneTestPerFileChangesetFromDirectoryStructure("sample/ChangesetOneTestPerFileSampleDir");

    EXPECT_TRUE(changeset->isChanged(123456u, "avr_encode_section_info"));
    EXPECT_TRUE(changeset->isChanged(123456u, "push_reload"));
    EXPECT_TRUE(changeset->isChanged(123456u, "push_replacement"));
    EXPECT_TRUE(changeset->isChanged(123456u, "subst_reloads"));
    EXPECT_TRUE(changeset->isChanged(123456u, "copy_replacements"));
    EXPECT_TRUE(changeset->isChanged(123456u, "copy_replacements_1"));
    EXPECT_TRUE(changeset->isChanged(123456u, "move_replacements"));
    EXPECT_TRUE(changeset->isChanged(123456u, "find_replacement"));
    EXPECT_TRUE(changeset->isChanged(123457u, "gimple_boolify"));
    EXPECT_TRUE(changeset->isChanged(123457u, "gimplify_expr"));
    EXPECT_TRUE(changeset->isChanged(123457u, "vect_vfa_segment_size"));
    EXPECT_TRUE(changeset->isChanged(123457u, "vect_create_cond_for_alias_checks"));
    EXPECT_TRUE(changeset->isChanged(123458u, "mips16_unextended_reference_p"));
    EXPECT_TRUE(changeset->isChanged(123458u, "mips_rtx_costs"));
    EXPECT_TRUE(changeset->isChanged(123458u, "mips_set_mips16_mode"));
    EXPECT_TRUE(changeset->isChanged(123458u, "mips_conditional_register_usage"));
    EXPECT_TRUE(changeset->isChanged(123458u, "mips_mulsidi3_gen_fn"));
    EXPECT_TRUE(changeset->isChanged(123458u, "foo"));
    EXPECT_TRUE(changeset->isChanged(123458u, "bar"));
    EXPECT_TRUE(changeset->isChanged(123458u, "_ITM_registerTMCloneTable"));
    EXPECT_TRUE(changeset->isChanged(123458u, "_ITM_deregisterTMCloneTable"));
    EXPECT_TRUE(changeset->isChanged(123459u, "diagnostic_finish"));
    EXPECT_TRUE(changeset->isChanged(123459u, "sched_init"));
    EXPECT_TRUE(changeset->isChanged(123459u, "sched_finish"));
    EXPECT_TRUE(changeset->isChanged(123460u, "do_ifdef"));
    EXPECT_TRUE(changeset->isChanged(123460u, "do_ifndef"));
    EXPECT_TRUE(changeset->isChanged(123460u, "parse_defined"));
    EXPECT_TRUE(changeset->isChanged(123461u, "build_static_cast_1"));
    EXPECT_TRUE(changeset->isChanged(123461u, "go_write_export_data"));
    EXPECT_TRUE(changeset->isChanged(123461u, "Stream_from_buffer::~Stream_from_buffer"));

    EXPECT_FALSE(changeset->isChanged(123456u, "gimple_boolify"));
    EXPECT_FALSE(changeset->isChanged(123456u, "gimplify_expr"));
    EXPECT_FALSE(changeset->isChanged(123456u, "vect_vfa_segment_size"));
    EXPECT_FALSE(changeset->isChanged(123456u, "vect_create_cond_for_alias_checks"));
    EXPECT_FALSE(changeset->isChanged(123456u, "mips16_unextended_reference_p"));
    EXPECT_FALSE(changeset->isChanged(123456u, "mips_rtx_costs"));
    EXPECT_FALSE(changeset->isChanged(123456u, "mips_set_mips16_mode"));
    EXPECT_FALSE(changeset->isChanged(123456u, "mips_conditional_register_usage"));
    EXPECT_FALSE(changeset->isChanged(123456u, "mips_mulsidi3_gen_fn"));
    EXPECT_FALSE(changeset->isChanged(123456u, "foo"));
    EXPECT_FALSE(changeset->isChanged(123456u, "bar"));
    EXPECT_FALSE(changeset->isChanged(123456u, "_ITM_registerTMCloneTable"));
    EXPECT_FALSE(changeset->isChanged(123456u, "_ITM_deregisterTMCloneTable"));
    EXPECT_FALSE(changeset->isChanged(123456u, "diagnostic_finish"));
    EXPECT_FALSE(changeset->isChanged(123456u, "sched_init"));
    EXPECT_FALSE(changeset->isChanged(123456u, "sched_finish"));
    EXPECT_FALSE(changeset->isChanged(123456u, "do_ifdef"));
    EXPECT_FALSE(changeset->isChanged(123456u, "do_ifndef"));
    EXPECT_FALSE(changeset->isChanged(123456u, "parse_defined"));
    EXPECT_FALSE(changeset->isChanged(123456u, "build_static_cast_1"));
    EXPECT_FALSE(changeset->isChanged(123456u, "go_write_export_data"));
    EXPECT_FALSE(changeset->isChanged(123456u, "Stream_from_buffer::~Stream_from_buffer"));
}
