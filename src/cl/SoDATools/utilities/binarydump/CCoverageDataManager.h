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

#ifndef CCOVERAGEDATAMANAGER_H
#define CCOVERAGEDATAMANAGER_H

#include "CDataManager.h"

namespace sodatools {

/**
 * @brief The CCoverageDataManager class contains methods for managing coverage data files.
 */
class CCoverageDataManager : public CDataManager
{
public:

    /**
     * @brief Constructor, creates an empty CCoverageDataManager object.
     */
    CCoverageDataManager();

    /**
     * @brief Creates a CCoverageDataManager object with a specified handler.
     * @param handler  CDataHandler object.
     */
    CCoverageDataManager(CDataHandler *handler);

    /**
     * @brief Destroys a CCoverageDataManager object.
     */
    ~CCoverageDataManager();

    /**
     * @brief Writes the content of a coverage data to a specified file.
     * @param filepath  File path.
     */
    void save(const String &filepath);

    /**
     * @brief Loads the content of a coverage data from a specified file.
     * @param filepath  File path.
     * @throw Exception if filepath is not a regular file.
     */
    void load(const String &filepath);

    /**
     * @brief Dumps the coverage matrix to a specified file.
     * @param filepath  File path.
     * @param psize  If true than writes the size of the matrix to the file.
     * @param csep  Column separator.
     * @param rsep  Record separator.
     */
    void dumpData(const String& filepath, bool psize=false, char csep=';', char rsep='\n');

    /**
     * @brief Dumps test case names to a specified file.
     * @param filepath  File path.
     */
    void dumpTestcases(const String &filepath);

    /**
     * @brief Dumps code element names to a specified file.
     * @param filepath  File path.
     */
    void dumpCodeElements(const String &filepath);
};

} // namespace sodatools

#endif /* CCOVERAGEDATAMANAGER_H */
