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

#include "CRevisionFilters.h"

namespace soda {

CRevisionFilters::CRevisionFilters()
{}

CRevisionFilters::~CRevisionFilters()
{}

IntVector CRevisionFilters::filterNonChangedOrNonFailed(IntVector& allrevs, CSelectionData* data, bool keepEmptyChanged, bool keepEmptyFailed) const
{
    IntVector revlist;
    for (IntVector::iterator revit = allrevs.begin(); revit != allrevs.end(); ++revit) {
        bool keepChanged = keepEmptyChanged ||
            (data->getChangeset()->exists(*revit)
             && data->getChangeset()->at(*revit).count() != 0); // Optimize: .empty()
        bool keepFailed = keepEmptyFailed;

        if (!keepFailed) {
            const IBitList& exec=data->getResults()->getExecutionBitList(*revit);
            const IBitList& pass=data->getResults()->getPassedBitList(*revit);
            IndexType tcs = exec.size();
            IndexType tid;
            for (tid = 0; tid < tcs; tid++) {
                if (exec[tid] && !pass[tid]) {
                    keepFailed = true; //There were failed TCs in the revision
                    break;
                }
            }
        }

        if (keepChanged && keepFailed) {
            revlist.push_back(*revit);
        }
    }
    return revlist;
}

IntVector CRevisionFilters::filterRange(IntVector &allrevs, IndexType first, IndexType last) const
{
    IntVector revlist;
    for (IndexType i = 0; i < allrevs.size(); i++) {
        if (first <= allrevs[i] && allrevs[i] < last) {
            revlist.push_back(allrevs[i]);
        }
    }
    return revlist;
}

} /* namespace soda */
