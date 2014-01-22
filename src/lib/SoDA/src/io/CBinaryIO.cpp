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

#include <iostream>
#include <fstream>
#include <sstream>
#include "io/CBinaryIO.h"
#include "exception/CIOException.h"

namespace soda { namespace io {

CBinaryIO::CBinaryIO() :
    m_file(NULL)
{}

CBinaryIO::CBinaryIO(const char *filename, eOpenMode mode) :
    m_file(NULL),
    m_mode(mode)
{
    open(filename, mode);
}

CBinaryIO::CBinaryIO(const String &filename, eOpenMode mode) :
    m_file(NULL),
    m_mode(mode)
{
    open(filename, mode);
}

CBinaryIO::~CBinaryIO()
{
    if (m_file != NULL) {
        delete m_file;
    }
}

void CBinaryIO::open(const String &filename, eOpenMode mode)
{
    open(filename.c_str(), mode);
}

void CBinaryIO::open(const char *filename, eOpenMode mode)
{
    if (m_file == NULL) {
        this->m_mode = mode;
        std::ios_base::openmode openMode;

        switch (mode) {
        case omRead:
            openMode = std::ios_base::in | std::ios_base::binary;
            break;
        case omWrite:
            openMode = std::ios_base::out | std::ios_base::binary;
            break;
        default:
            openMode = std::ios_base::in | std::ios_base::binary;
            break;
        }

        try {
            m_file = new std::fstream(filename, openMode);
            if (m_file->fail()) {
                throw CIOException("soda::io::BinaryIO::open()", "Can not open file: " + String(filename));
            }
        } catch (std::ios_base::failure e) {
            throw CIOException("soda::io::BinaryIO::open()", e.what());
        }


    }
}


bool CBinaryIO::isOpen()
{
    return m_file != NULL && m_file->is_open();
}

void CBinaryIO::close()
{
    if (!isOpen()) {
        throw CIOException("soda::io::BinaryIO::close()", "The file is not open.");
    }
    try {
        m_file->close();
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::close()", e.what());
    }
    delete m_file;
    m_file = NULL;
}

bool CBinaryIO::eof()
{
    return m_file != NULL && m_file->eof();
}

void CBinaryIO::writeBool1(bool b)
{
    if (!isWritable()) {
        throw CIOException("soda::io::BinaryIO::writeBool1()", "The file is not writable");
    }
    char c = b ? 1 : 0;
    try {
        m_file->write(&c, 1);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::writeBool1()", e.what());
    }
}

bool CBinaryIO::readBool1()
{
    if (!isReadable()) {
        throw CIOException("soda::io::BinaryIO::readBool1()", "The file is not readable");
    }
    char b = 0;
    try {
        m_file->read(&b, 1);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::readBool1()", e.what());
    }
    return b == 1;
}

void CBinaryIO::writeByte1(char c)
{
    if (!isWritable()) {
        throw CIOException("soda::io::BinaryIO::writeByte1()", "The file is not writable");
    }
    try {
        m_file->write(&c, 1);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::writeByte1()", e.what());
    }
}

char CBinaryIO::readByte1()
{
    if (!isReadable()) {
        throw CIOException("soda::io::BinaryIO::readByte1()", "The file is not readable");
    }
    char c;
    try {
        m_file->read(&c, 1);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::readByte1()", e.what());
    }

    return c;
}

void CBinaryIO::writeUByte1(unsigned char c)
{
    if (!isWritable()) {
        throw CIOException("soda::io::BinaryIO::writeUByte1()", "The file is not writable");
    }
    try {
        m_file->write((char *)&c, 1);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::writeUByte1()", e.what());
    }
}

unsigned char CBinaryIO::readUByte1()
{
    if (!isReadable()) {
        throw CIOException("soda::io::BinaryIO::readUByte1()", "The file is not readable");
    }
    unsigned char c;
    try {
        m_file->read((char *)&c, 1);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::readUByte1()", e.what());
    }

    return c;
}

void CBinaryIO::writeInt4(int i)
{
    if (!isWritable()) {
        throw CIOException("soda::io::BinaryIO::writeInt4()", "The file is not writable");
    }
    try {
        m_file->write((char *)&i, 4);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::writeInt4()", e.what());
    }
}

int CBinaryIO::readInt4()
{
    if (!isReadable()) {
        throw CIOException("soda::io::BinaryIO::readInt4()", "The file is not readable");
    }
    int i;
    try {
        m_file->read((char *)&i, 4);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::readInt4()", e.what());
    }

    return i;
}

void CBinaryIO::writeUInt4(unsigned i)
{
    if (!isWritable()) {
        throw CIOException("soda::io::BinaryIO::writeUInt4()", "The file is not writable");
    }
    try {
        m_file->write((char *)&i, 4);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::writeUInt4()", e.what());
    }
}

unsigned CBinaryIO::readUInt4()
{
    if (!isReadable()) {
        throw CIOException("soda::io::BinaryIO::readUInt4()", "The file is not readable");
    }
    unsigned i;
    try {
        m_file->read((char *)&i, 4);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::readUInt4()", e.what());
    }

    return i;
}

void CBinaryIO::writeLongLong8(long long i)
{
    if (!isWritable()) {
        throw CIOException("soda::io::BinaryIO::writeLongLong8()", "The file is not writable");
    }
    try {
        m_file->write((char *)&i, 8);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::writeLongLong8()", e.what());
    }
}

long long CBinaryIO::readLongLong8()
{
    if (!isReadable()) {
        throw CIOException("soda::io::BinaryIO::readLongLong8()", "The file is not readable");
    }
    long long i;
    try {
        m_file->read((char *)&i, 8);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::readLongLong8()", e.what());
    }

    return i;
}

void CBinaryIO::writeULongLong8(unsigned long long i)
{
    if (!isWritable()) {
        throw CIOException("soda::io::BinaryIO::writeULongLong8()", "The file is not writable");
    }
    try {
        m_file->write((char *)&i, 8);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::writeULongLong8()", e.what());
    }
}

unsigned long long CBinaryIO::readULongLong8()
{
    if (!isReadable()) {
        throw CIOException("soda::io::BinaryIO::readULongLong8()", "The file is not readable");
    }
    unsigned long long i;
    try {
        m_file->read((char *)&i, 8);
    } catch (std::ios_base::failure e) {
        throw CIOException("soda::io::BinaryIO::readULongLong8()", e.what());
    }

    return i;
}

void CBinaryIO::writeString(const String &s)
{
    if (!isWritable()) {
        throw CIOException("soda::io::BinaryIO::writeString()", "The file is not writable");
    }
    size_t len = s.size();
    try {
      m_file->write(s.c_str(),(std::streamsize)len+1);
    } catch(std::ios_base::failure e) {
      throw CIOException("soda::io::BinaryIO::writeString()", e.what());
    }
}

const String CBinaryIO::readString()
{
    if (!isReadable()) {
        throw CIOException("soda::io::BinaryIO::readString()", "The file is not readable");
    }
    try {
      std::stringbuf ss;
      int testNotEmpty = m_file->peek();
      if (testNotEmpty){
        m_file->get(ss, '\0');
      }
      // read string end 0
      m_file->get();
      return ss.str();
    } catch(std::ios_base::failure e) {
      throw CIOException("soda::io::BinaryIO::readString()", e.what());
    }
}

void CBinaryIO::writeData(const void *data, std::streamsize size)
{
    if (!isWritable()) {
        throw CIOException("soda::io::BinaryIO::writeData()", "The file is not writable");
    }
    try {
        m_file->write((char *)data, size);
    } catch(std::ios_base::failure e) {
      throw CIOException("soda::io::BinaryIO::writeData()", e.what());
    }
}

void CBinaryIO::readData(void *data, std::streamsize size)
{
    if (!isReadable()) {
        throw CIOException("soda::io::BinaryIO::readData()", "The file is not readable");
    }
    try {
      m_file->read((char *)data, size);
    } catch(std::ios_base::failure e) {
      throw CIOException("soda::io::BinaryIO::readData()", e.what());
    }
}

bool CBinaryIO::isWritable()
{
    if (isOpen() && m_mode == omWrite) {
        return true;
    }
    return false;
}

bool CBinaryIO::isReadable()
{
    if (isOpen() && m_mode == omRead) {
        return true;
    }
    return false;
}

} /* namespace io */

} /* namespace soda */
