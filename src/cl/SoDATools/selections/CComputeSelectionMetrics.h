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
#include "engine/CKernel.h"

namespace soda {

/**
 * @brief The CComputeSelectionMetrics class measures the specified selection data
 *        with a choosen sorting algorithm.
 */
class CComputeSelectionMetrics
{
public:
    /**
     * @brief The SelectionMetrics class stores the results of the measurements
     *         made on the specified selection data.
     */
    class SelectionMetrics
    {
    public:
        /**
         * @brief Creates a new SelectionData object.
         */
        SelectionMetrics();
        ~SelectionMetrics();

        /**
         * @brief Adds the values of the specified selection data to the current object.
         * @param rhs SelectionMetrics object.
         */
        inline SelectionMetrics& operator+=(const SelectionMetrics& rhs) {
            if (this == &rhs)
                return *this;

            nofSelected += rhs.nofSelected;
            nofFailed += rhs.nofFailed;
            nofHit += rhs.nofHit;
            return *this;
        }

        /**
         * @brief Returns the inclusiveness of the selection data.
         * @return Number of executed test cases divided by the number of failed test cases.
         */
        inline double inclusiveness() {
            return (nofFailed) ? ((double)nofHit / (double)nofFailed) : (1.0);
        }

        /**
         * @brief Returns the precision of the selection data.
         * @return Number of executed test cases divided by the number of selected test cases.
         */
        inline double precision() {
            return (nofSelected) ?((double)nofHit / (double)nofSelected) : (1.0);
        }

        /**
         * @brief Computes a value from precision and inclusiveness values.
         * @return Value computed from precision and inclusiveness.
         */
        inline double f_measure() {
            return (precision() + inclusiveness() > 0.0) ?
                    (2 * (precision() * inclusiveness()) / (precision() + inclusiveness())) : (0.0);
        }

        /**
         * @brief Number of selected test cases.
         */
        IndexType nofSelected;

        /**
         * @brief Number of failed test cases.
         */
        IndexType nofFailed;

        /**
         * @brief Number of executed test cases.
         */
        IndexType nofHit;
    };

    /**
     * @brief Creates a new CComputeSelectionMetrics object.
     * @param dbg Progress bar level.
     */
    CComputeSelectionMetrics(CSelectionData*, ITestSuitePrioritizationPlugin*, IntVector*, IntVector*, int dbg = 0);
    ~CComputeSelectionMetrics();

    /**
     * @brief Measures the data of a specified selection.
     * @param rev Revision number.
     * @param pdata Stores the measured data.
     * @param siz Selection size.
     * @param sdata Stores the measured data.
     */
    void runMeasurementForOneSelectionSize(RevNumType rev, SelectionMetrics* pdata, int size, SelectionMetrics* sdata);

    /**
     * @brief Measures the data of a specified revision.
     * @param rev Revision number.
     * @param pdata Stores the measured data.
     */
    void runMeasurementForOneRevision(RevNumType rev, SelectionMetrics* pdata);

    /**
     * @brief Measures the given data.
     */
    void runMeasurement();

    /**
     * @brief Prints the results of measures.
     */
    String getData();

    /**
     * @brief Prints detailed results of measures.
     */
    String getDetailedData();

private:

    /**
     * @brief Data for measures.
     */
    CSelectionData* m_data;

    /**
     * @brief Sorting method.
     */
    ITestSuitePrioritizationPlugin* m_prioAlg;

    /**
     * @brief List of revision numbers.
     */
    IntVector* m_revisionList;

    /**
     * @brief List of selection sizes.
     */
    IntVector* m_sizeList;

    /**
     * @brief Number of revisions.
     */
    size_t m_numberOfRevisions;

    /**
     * @brief Number of selections.
     */
    size_t m_numberOfSelections;

    /**
     * @brief Stores measured data in an array for every revision grouped by the number of selections.
     */
    SelectionMetrics* m_perRevisionData;

    /**
     * @brief Stores measured data in an array for every selection.
     */
    SelectionMetrics* m_sumTestcaseData;

    /**
     * @brief Translation error counter.
     */
    unsigned int e_translation;

    /**
     * @brief Progress bar level.
     */
    unsigned int d_progress_bar;
};

} /* namespace soda */

#endif /* CCOMPUTESELECTIONMETRICS_H */
