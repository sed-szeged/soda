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

#ifndef CCOMPUTESELECTIONMETRICS_H
#define CCOMPUTESELECTIONMETRICS_H

#include "data/CSelectionData.h"
#include "plugins/prioritization/IPrioritizationPlugin.h"

namespace soda {

class CComputeSelectionMetrics
{
public:
    class SelectionData
    {
    public:
        SelectionData();

        ~SelectionData();

        inline void add(const SelectionData& rhs) {
            nofSelected += rhs.nofSelected;
            nofFailed += rhs.nofFailed;
            nofHit += rhs.nofHit;
        }

        inline double inclusiveness() {
            return (nofFailed) ? ((double)nofHit / (double)nofFailed) : (1.0);
        }

        inline double precision() {
            return (nofSelected) ?((double)nofHit / (double)nofSelected) : (1.0);
        }

        inline double f_measure() {
            return (precision() + inclusiveness() > 0.0) ?
                    (2 * (precision() * inclusiveness()) / (precision() + inclusiveness())) : (0.0);
        }

        IndexType nofSelected;
        IndexType nofFailed;
        IndexType nofHit;
    };

    CComputeSelectionMetrics(CSelectionData*, IPrioritizationPlugin*, IntVector*, IntVector*, int dbg = 0);

    ~CComputeSelectionMetrics();

    void runMeasurementForOneSelectionSize(RevNumType rev, SelectionData* pdata, int siz, SelectionData* sdata);

    void runMeasurementForOneRevision(RevNumType rev, SelectionData* pdata);

    void runMeasurement();

    void printData();

    void printDetailedData();

private:

    CSelectionData* m_data;

    IPrioritizationPlugin* m_prioAlg;

    IntVector* m_revisionList;

    IntVector* m_sizeList;

    size_t m_numberOfRevisions;

    size_t m_numberOfSelections;

    SelectionData* m_perRevisionData;

    SelectionData* m_sumTestcaseData;

    unsigned int e_translation;

    unsigned int d_progress_bar;
};

} /* namespace soda */

#endif /* CCOMPUTESELECTIONMETRICS_H */
