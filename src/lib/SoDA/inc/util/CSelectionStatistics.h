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

#include "data/CSelectionData.h"
#include "rapidjson/document.h"

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
    CSelectionStatistics(CSelectionData *data);

    /**
     * @brief Destroys a CSelectionStatistics object.
     */
    ~CSelectionStatistics();

    /**
     * @brief Calculates coverage statistics.
     */
    rapidjson::Document calcCoverageRelatedStatistics();

    /**
     * @brief Calculates changeset statistics.
     */
    void calcChangeRelatedStatistics();

    /**
     * @brief Calculates failed test case statistics.
     */
    void calcFailStatistics(rapidjson::Document &doc);

    /**
     * @brief Calculates results summary.
     */
    rapidjson::Document calcCovResultsSummary();

    /**
     * @brief Converts the given data to json array.
     * @param data Data to convert.
     * @return Returns the converted json file.
     */
    void toJson(IdxIdxMap &data, rapidjson::Value& val, rapidjson::Document &root);

private:

    /**
     * @brief Stores selection data.
     */
    CSelectionData *m_selectionData;
};

} // namespace soda

#endif /* CSELECTIONSTATISTICS_H */
