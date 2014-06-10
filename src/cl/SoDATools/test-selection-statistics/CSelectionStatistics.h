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

#ifndef CSELECTIONSTATISTICS_H
#define CSELECTIONSTATISTICS_H

#include <fstream>
#include <iostream>

#include "CDataManager.h"

namespace soda {
/**
 * @brief The CSelectionStatistics class contains methods for calculating statistics on selection data.
 */
class CSelectionStatistics
{
public:

    /**
     * @brief Constructor, creates a CSelectionStatistics object.
     * @param mgr CDataManager object.
     */
    CSelectionStatistics(CDataManager *mgr);

    /**
     * @brief Destroys a CSelectionStatistics object.
     */
    ~CSelectionStatistics();

    /**
     * @brief Calculates coverage statistics.
     */
    void calcCoverageRelatedStatistics();

    /**
     * @brief Calculates changeset statistics.
     */
    void calcChangeRelatedStatistics();

    /**
     * @brief Calculates failed test case statistics.
     */
    void calcFailStatistics();

    /**
     * @brief Calculates results summary.
     */
    void calcCovResultsSummary();

    /**
     * @brief Writes the given data into a csv file.
     * @param out Output stream.
     * @param data Data to write.
     */
    void writeCsv(std::ofstream& out, IdxIdxMap data);

    /**
     * @brief Returns a pointer to selection data in m_dataManager.
     * @return Pointer to selection data in m_dataManager.
     */
    CSelectionData* getSelectionData();

private:

    /**
     * @brief CDataManager object which contains selection data and other test selection statistics parameters.
     */
    CDataManager *m_dataManager;
};

} // namespace soda

#endif /* CSELECTIONSTATISTICS_H */
