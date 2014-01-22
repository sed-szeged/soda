/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: László Langó <lango@inf.u-szeged.hu>
 *          Tamás Gergely <gertom@inf.u-szeged.hu>
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

#ifndef CREADCHANGESET_H
#define CREADCHANGESET_H

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include "data/CChangeset.h"
#include "boost/filesystem.hpp"

using namespace soda;

namespace sodatools {

/**
 * @brief The CReadChangeset class reads raw change set data from directories.
 */
class CReadChangeset {
public:
    /**
     * @brief Creates a CReadChangeset object.
     * @param changeset  Specified change set.
     * @param read_ce  If true than reads the code element names.
     * @param verbose  If true than methods will print information to the standard output.
     */
    CReadChangeset(CChangeset* changeset, bool read_ce=true, bool verbose=true);

    /**
     * @brief Destroys a CReadChangeset object.
     */
    ~CReadChangeset();

    /**
     * @brief Reads One Test per File format change set data from the specified path.
     * @param path  Directory path.
     * @throw Exception at invalid directory path.
     */
    virtual void readOneTestPerFileChangesetFromDirectoryStructure(const char * path);

    /**
     * @brief Reads One Test per File format change set data from the specified path.
     * @param path  Directory path.
     */
    virtual void readOneTestPerFileChangesetFromDirectoryStructure(const String& path);

    /**
     * @brief Set the value of the m_verbose variable to v.
     * @param v  Value to be set.
     */
    inline void setVerbose(bool v) {m_verbose = v;}

private:

    /**
     * @brief Reads One Test per File format code element names from the specified directory.
     *        Recursively searches the files in the given path p
     *        File format is: <CodeElementName>,<SourceFilePath>
     *        <CodeElementName> is used as the codeElement name
     */
    virtual void readOneTestPerFileChangesetFromDirectory1stPass(boost::filesystem::path);

    /**
     * @brief Reads One Test per File format change set data from the specified directory.
     *        Recursively searches the files in the given path p
     *        File name format: <Dirname>/<Basename>.<Extension>
     *        <Basename> is used as the revision number, <Dirname> and <Extension> are not used
     *        File format is: <CodeElementName>,<SourceFilePath>
     *        <CodeElementName> is used as the codeElement name
     * @throw Exception at invalid code element name.
     */
    virtual void readOneTestPerFileChangesetFromDirectory(boost::filesystem::path);

private:

    /**
     * @brief Stores change set data.
     */
    CChangeset *m_changeset;

    /**
     * @brief If true than reads the code element names.
     */
    bool        m_readCodeElements;

    /**
     * @brief If true than methods will print information to the standard output.
     */
    bool        m_verbose;
};

} // namespace sodatools

#endif /* CREADCHANGESET_H */
