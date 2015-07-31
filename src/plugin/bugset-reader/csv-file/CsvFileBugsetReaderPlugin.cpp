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

#include <fstream>
#include <iostream>

#include "CsvFileBugsetReaderPlugin.h"

#include "boost/algorithm/string_regex.hpp"
#include "boost/regex.hpp"
#include "boost/lexical_cast.hpp"
#include "exception/CException.h"


namespace soda {

CsvFileBugsetReaderPlugin::CsvFileBugsetReaderPlugin() :
    m_bugset(NULL)
{}

CsvFileBugsetReaderPlugin::~CsvFileBugsetReaderPlugin()
{}

std::string CsvFileBugsetReaderPlugin::getName()
{
    return "csv-file";
}

std::string CsvFileBugsetReaderPlugin::getDescription()
{
    return "The bug information is in csv files and the format is: bug_id, bug_url, bug_report_timestamp, fix_hash, fix_timestamp, method_name, method_qualified_name, file, line.";
}

CBugset* CsvFileBugsetReaderPlugin::read(const std::string &path)
{
    m_bugset = new CBugset();
    readFromDirectoryStructure(path);
    return m_bugset;
}

void CsvFileBugsetReaderPlugin::readFromDirectoryStructure(const char * dirname)
{
    fs::path bugs_path(dirname);
    if (!(exists(bugs_path) && is_directory(bugs_path))) {
        throw CException("CsvFileBugsetReaderPlugin::readFromDirectoryStructure()", "Specified path does not exists or is not a directory.");
    }
    readFromDirectory(bugs_path);
}

void CsvFileBugsetReaderPlugin::readFromDirectoryStructure(const std::string& dirname)
{
    readFromDirectoryStructure(dirname.c_str());
}

void CsvFileBugsetReaderPlugin::readFromDirectory(fs::path p)
{
    std::cout << "Directory: " << p << std::endl;
    std::cout.flush();

    std::vector<fs::path> pathVector;
    std::copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(pathVector));
    std::sort(pathVector.begin(), pathVector.end());

    for (std::vector<fs::path>::iterator it = pathVector.begin(); it != pathVector.end(); it++) {
        if (is_directory(*it)) { // recurse into subdirs
            if (basename(*it) != "")
                readFromDirectory(*it);
        } else if (it->extension() == ".csv") {
            std::ifstream in(it->string().c_str());

            // line format: bug_id,"bug_url","bug_report_timestamp","fix_hash","fix_timestamp", "method_name","method_qualified_name","file","line"
            String line;
            StringVector data;
            std::getline(in, line); // consume first line
            while (std::getline(in, line)) {
                boost::algorithm::split_regex(data, line, boost::regex("\",\""));
                if (data.size() != 9u) {
                    continue;
                }

                RevNumType const id = boost::lexical_cast<RevNumType>(data[0].substr(1));
                Report rData;
                boost::regex timeRe("000$");
                auto time = boost::regex_replace(data[4], timeRe, "");
                rData.fixTime = boost::lexical_cast<time_t>(time);
                time = boost::regex_replace(data[2], timeRe, "");
                rData.reportTime = boost::lexical_cast<time_t>(time);
                boost::regex re("\\.");
                auto ceName = boost::regex_replace(data[6], re, "/");
                m_bugset->addReported(data[3], ceName, id, rData);
                data.clear();
            }
            in.close();
        }
    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getBugsetReaderPluginManager().addPlugin(new CsvFileBugsetReaderPlugin());
}

} /* namespace soda */
 