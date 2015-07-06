/*
 * Copyright (C): 2015 Department of Software Engineering, University of Szeged
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


#ifndef CSVFILEBUGSETREADERPLUGIN_H
#define CSVFILEBUGSETREADERPLUGIN_H

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include "boost/filesystem.hpp"
#include "engine/CKernel.h"

namespace fs = boost::filesystem;

namespace soda {

/**
 * @brief The CsvFileBugsetReaderPlugin class implements methods which reads raw
 *        issue report datas from csv files with a specified format.
 *        Reads all files from a given directory.
 */
class CsvFileBugsetReaderPlugin : public IBugsetReaderPlugin
{
public:

    CsvFileBugsetReaderPlugin();
    virtual ~CsvFileBugsetReaderPlugin();

    std::string getName();
    std::string getDescription();

    CBugset* read(const std::string &path);

private:

    void readFromDirectoryStructure(const char * path);


    void readFromDirectoryStructure(const std::string& path);

    void readFromDirectory(fs::path);

    /**
     * @brief Stores issue data data.
     */
    CBugset *m_bugset;
};

} /* namespace soda */

#endif /* CSVFILEBUGSETREADERPLUGIN_H */
