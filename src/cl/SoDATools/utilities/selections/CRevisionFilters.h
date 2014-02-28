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

#ifndef CREVISIONFILTERS_H
#define CREVISIONFILTERS_H

#include "data/CSelectionData.h"

namespace soda {

/**
 * @brief The CRevisionFilters class contains methods for revision filtering.
 */
class CRevisionFilters {
public:

    /**
     * @brief Creates a new CRevisionFilters object.
     */
    CRevisionFilters();
    ~CRevisionFilters();

    /**
     * @brief Filters non changed or non failed revision numbers.
     * @return Filtered revision numbers.
     */
    IntVector filterNonChangedOrNonFailed(IntVector& , CSelectionData*, bool, bool) const;

    /**
     * @brief Returns the filtered revision numbers which are in the specified interval.
     * @param allrevs Revision numbers.
     * @param first Lower revision limit.
     * @param last Upper revision limit.
     * @return Filtered revision numbers.
     */
    IntVector filterRange(IntVector &allrevs, IndexType first, IndexType last) const;
};

} /* namespace soda */

#endif /* CREVISIONFILTERS_H */
