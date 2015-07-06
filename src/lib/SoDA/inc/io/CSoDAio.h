/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: László Langó <>
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

#ifndef CSODAIO_H
#define CSODAIO_H

#include "io/CBinaryIO.h"
#include "data/SoDALibDefs.h"

namespace soda { namespace io {

/**
 * @brief The CSoDAio class implements a binary reader/writer specialised for SoDA file formats.
 */
class CSoDAio : public io::CBinaryIO {
public:

    /**
     * @brief Constructor, creates an empty CSoDAio.
     */
    CSoDAio();

    /**
     * @brief Creates a CSoDAio object and stores the file name and the specified open mode.
     * @param filename File name.
     */
    CSoDAio(const char* filename, io::CBinaryIO::eOpenMode);

    /**
     * @brief Creates a CSoDAio object and stores the file name and the specified open mode.
     * @param filename File name.
     */
    CSoDAio(const String& filename, io::CBinaryIO::eOpenMode);

    /**
     * @brief Destroys a CSoDAio object.
     */
    ~CSoDAio();

    enum ChunkID {
        UNKNOWN_TYPE = 0,
        COVERAGE = 123456,
        RELATION,
        IDMANAGER,
        TCLIST,
        PRLIST,
        REVISIONS,
        BITMATRIX,
        BITLIST,
        EXECUTION,
        PASSED,
        CHANGESET,
        CODEELEMENT_TRACE,
        REVLIST,
        BUGSET
    };

    /**
     * @brief Opens a file, if a file is already opened than it'll close that file.
     * @param filename File name.
     */
    void open(const char* filename, io::CBinaryIO::eOpenMode);

    /**
     * @brief Opens a file, if a file is already opened than it'll close that file.
     * @param filename File name.
     */
    void open(const String& filename, io::CBinaryIO::eOpenMode);

    /**
     * @brief Closes an opened file.
     */
    void close();

    /**
     * @brief Returns the length of the current chunk.
     * @return Length of the current chunk.
     */
    const unsigned long long int getActualLength() const;

    /**
     * @brief Returns the current chunk id.
     * @return Current chunk id.
     */
    const ChunkID getChunkID() const;

    /**
     * @brief Reads the next chunk id and length of the chunk from the file.
     * @return True if we are not at the end of stream.
     * @throw Exception at unexpected end of file.
     */
    bool nextChunkID();

    /**
     * @brief Returns true if the specified chunk id is in the current file.
     * @param chunkID Chunk id.
     * @return True if the specified chunk id is in the current file.
     * @throw Exception if there is no opened file.
     * @throw Exception if open mode is not omRead.
     */
    bool findChunkID(ChunkID chunkID);

private:

    /**
     * @brief Identifier of the file chunk.
     */
    ChunkID m_chunkID;

    /**
     * @brief Length of the file chunk.
     */
    unsigned long long int m_length;

    /**
     * @brief Current position in the file.
     */
    std::streampos m_lastpos;

private:

    /**
     * @brief NIY copy constructor.
     */
    CSoDAio(const CSoDAio&);

    /**
     * @brief NIY operator =.
     * @return
     */
    CSoDAio& operator=(const CSoDAio&);

private:

    /**
     * @brief Identifier of SoDA files.
     */
    static const unsigned int SoDA_MAGIC;

    /**
     * @brief Checks if the current file with the specified open mode is a standard SoDA file.
     * @throw Exception if file magic number is not equal to SoDA magic number.
     * @throw Exception at invalid open mode.
     */
    void checkOpened(io::CBinaryIO::eOpenMode);
};

} /* namespace io */

} /* namespace soda */

#endif /* CSODAIO_H */
