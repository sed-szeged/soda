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

class CRevisionFilters {
public:
    CRevisionFilters();
    ~CRevisionFilters();

    /**
     * Filters the revision list according to the settings
     *
     */
    IntVector filter(IntVector& , CSelectionData*, bool, bool) const;
    IntVector filter(IntVector &allrevs, IndexType first, IndexType last) const;
};

} /* namespace soda */

#endif /* CREVISIONFILTERS_H */
