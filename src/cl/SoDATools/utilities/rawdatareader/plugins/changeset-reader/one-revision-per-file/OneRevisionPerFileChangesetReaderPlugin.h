/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Tengeri <dtengeri@inf.u-szeged.hu>
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

#ifndef ONEREVISIONPERFILECHANGESETREADERPLUGIN_H
#define ONEREVISIONPERFILECHANGESETREADERPLUGIN_H

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include "boost/filesystem.hpp"
#include "../IChangesetReaderPlugin.h"

namespace soda {

/**
 * @brief The OneRevisionPerFileChangesetReaderPlugin class implements methods which reads raw
 *        one revision per file data and converts it to changeset data.
 */
class OneRevisionPerFileChangesetReaderPlugin : public IChangesetReaderPlugin
{
public:

    OneRevisionPerFileChangesetReaderPlugin();
    ~OneRevisionPerFileChangesetReaderPlugin();

    std::string getName();
    std::string getDescription();

    CChangeset* read(const std::string &path);

private:
    /**
     * @brief Reads One Revision per File format changeset data from the specified path.
     * @param path  Directory path.
     * @throw Exception at invalid directory path.
     */
    void readFromDirectoryStructure(const char * path);

    /**
     * @brief Reads One Revision per File format change set data from the specified path.
     * @param path  Directory path.
     */
    void readFromDirectoryStructure(const String& path);

    /**
     * @brief Reads One Revision per File format code element names from the specified directory.
     *        Recursively searches the files in the given path p
     *        File format is: <CodeElementName>,<SourceFilePath>
     *        <CodeElementName> is used as the codeElement name
     */
    void readFromDirectory1stPass(boost::filesystem::path);

    /**
     * @brief Reads One Revision per File format change set data from the specified directory.
     *        Recursively searches the files in the given path p
     *        File name format: <Dirname>/<Basename>.<Extension>
     *        <Basename> is used as the revision number, <Dirname> and <Extension> are not used
     *        File format is: <CodeElementName>,<SourceFilePath>
     *        <CodeElementName> is used as the codeElement name
     * @throw Exception at invalid code element name.
     */
    void readFromDirectory(boost::filesystem::path);

    /**
     * @brief Stores change set data.
     */
    CChangeset *m_changeset;
};

} /* namespace soda */

#endif /* ONEREVISIONPERFILECHANGESETREADERPLUGIN_H */
