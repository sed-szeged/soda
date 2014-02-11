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

#include <fstream>

#include "io/CSoDAio.h"
#include "exception/CException.h"

namespace soda { namespace io {

const unsigned int CSoDAio::SoDA_MAGIC = 0x41446f53; // LSB: 'S', 'o', 'D', 'A'

void CSoDAio::checkOpened(const char* filename, io::CBinaryIO::eOpenMode openMode)
{
    if(openMode == io::CBinaryIO::omRead) {
        unsigned int id = readUInt4();
        if(id != SoDA_MAGIC) {
            CBinaryIO::close();
            throw CException("soda::CSoDAio::open()","Failed to open file! This is not a standard SoDA library file!");
        }
    } else if(openMode == io::CBinaryIO::omWrite) {
        writeUInt4(SoDA_MAGIC);
    } else {
        CBinaryIO::close();
        throw CException("soda::CSoDAio::open()","Not supported eOpenMode! (Use one of these: omRead, omWrite, omAppend)");
    }
    m_lastpos = m_file->tellg();
}

CSoDAio::CSoDAio() :
    m_chunkID(CSoDAio::UNKNOWN_TYPE),
    m_length(0),
    m_lastpos(0)
{ }

CSoDAio::CSoDAio(const char* filename, io::CBinaryIO::eOpenMode openMode) :
    io::CBinaryIO(filename, openMode),
    m_chunkID(CSoDAio::UNKNOWN_TYPE),
    m_length(0),
    m_lastpos(0)
{
    checkOpened(filename, openMode);
}

CSoDAio::CSoDAio(const String& filename, io::CBinaryIO::eOpenMode openMode) :
    io::CBinaryIO(filename, openMode),
    m_chunkID(CSoDAio::UNKNOWN_TYPE),
    m_length(0),
    m_lastpos(0)
{
    checkOpened(filename.c_str(), openMode);
}

CSoDAio::~CSoDAio()
{
}

void CSoDAio::open(const char* filename, io::CBinaryIO::eOpenMode openMode)
{
    if(isOpen()) {
        CBinaryIO::close();
    }
    CBinaryIO::open(filename, openMode);
    checkOpened(filename, openMode);
}

void CSoDAio::open(const String& filename, io::CBinaryIO::eOpenMode openMode)
{
    open(filename.c_str(), openMode);
}

void CSoDAio::close()
{
    if(isOpen()) {
        CBinaryIO::close();
    }
}

const unsigned long long int CSoDAio::getActualLength() const
{
    return m_length;
}

const CSoDAio::ChunkID CSoDAio::getChunkID() const
{
    return m_chunkID;
}

bool CSoDAio::nextChunkID()
{
    if(m_file->tellg() == m_lastpos) {
        for(unsigned long long int i = 0; i < m_length; ++i) {
            readByte1();
            if(eof()) {
                throw CException("soda::CSoDAio::nextChunkID()", "Unexpected end of file!");
            }
        }
    }

    /*
     * WARNING: the results are not defined if the integer value is outside the range
     * of the defined enumeration
     */
    m_chunkID = (CSoDAio::ChunkID)readInt4();
    m_length = readULongLong8();
    if(eof()) {
        return false;
    } else {
        m_lastpos = m_file->tellg();
        return true;
    }
}

bool CSoDAio::findChunkID(ChunkID chunkID)
{
    if(!isOpen())
        throw CException("soda::CSoDAio::findChunkID()","File is not open!");
    if(m_mode != omRead)
        throw CException("soda::CSoDAio::findChunkID()","File open mode isn't 'omRead'! You can use findChunkID only in omRead mode!");

    m_file->seekg(4, std::ios::beg); // SoDA_MAGIC on first 4 bytes

    while(nextChunkID()){
        if(m_chunkID == chunkID) {
            return true;
        }
    }
    return false;
}

} /* namespace io */

} /* namespace soda */
