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

#ifndef CDATAMANAGER_H
#define CDATAMANAGER_H

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include "CDataHandler.h"
#include "boost/filesystem.hpp"

using namespace boost::filesystem;

namespace sodatools {

class CDataHandler;

/**
 * @brief The CDataManager class contains methods for managing multiple data types.
 */
class CDataManager
{
public:

    /**
     * @brief Constructor, creates an empty CDataManager object.
     */
    CDataManager();

    /**
     * @brief Creates a CDataManager object with a specified handler.
     * @param handler  CDataHandler object.
     */
    CDataManager(CDataHandler* handler);

    /**
     * @brief Destroys a CDataManager object.
     */
    ~CDataManager();

    /**
     * @brief Saves the content of a data type.
     * @param filepath  File path.
     */
    virtual void save(const String& filepath) = 0;

    /**
     * @brief Loads binary data from a specified file.
     * @param filepath  File path.
     */
    virtual void load(const String& filepath) = 0;

    /**
     * @brief Returns the read format type.
     * @return Read format type.
     */
    ReadFormat getReadFormat() const;

    /**
     * @brief Returns the value of CDataHandler::m_bPrintInfo.
     * @return The value of CDataHandler::m_bPrintInfo.
     */
    bool getPrintInfo() const;

    /**
     * @brief Returns the value of CDataHandler::m_bWithPassFail.
     * @return The value of CDataHandler::m_bWithPassFail.
     */
    bool getWithPassFail() const;

    /**
     * @brief Returns the value of CDataHandler::m_bWithNames.
     * @return The value of CDataHandler::m_bWithNames.
     */
    bool getWithNames() const;

    /**
     * @brief Checks if the specified path is valid.
     * @param p  Path to be checked.
     * @return 0: Invalid path, 1: File, 2: Directory.
     */
    static int checkPathOk(path p);

    /**
     * @brief Returns a pointer to m_pDataHandler object.
     * @return Pointer to m_pDataHandler object.
     */
    CDataHandler *getDataHandler();
private:

    /**
     * @brief Stores containers for multiple data types.
     */
    CDataHandler *m_pDataHandler;
};

} // namespace sodatools

#endif /* CDATAMANAGER_H */
