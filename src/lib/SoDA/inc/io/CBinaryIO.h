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

#ifndef CBINARYIO_H
#define CBINARYIO_H

#include <ios>
#include "data/SoDALibDefs.h"

namespace soda { namespace io {

/**
 * @brief The CBinaryIO class implements a binary file reader/writer.
 */
class CBinaryIO
{
public:
    enum eOpenMode {
        omRead,
        omWrite
    };

    /**
     * @brief Constructor, creates empty BinaryIO object.
     */
    CBinaryIO();

    /**
     * @brief Creates BinaryIO object and open a file.
     * @param filename  File name.
     * @param mode  Open mode.
     * @throw IOException if open is failed.
     */
    CBinaryIO(const char *filename, eOpenMode m_mode);

    /**
     * @brief Creates BinaryIO object and open a file.
     * @param filename  File name.
     * @param mode  Open mode.
     * @throw IOException if open is failed.
     */
    CBinaryIO(const String& filename, eOpenMode m_mode);

    /**
     * @brief Destroys a CBinaryIO object.
     */
    virtual ~CBinaryIO();

    /**
     * @brief Opens a file, if a file is already opened it will be closed.
     * @param filename  File name.
     * @param mode  Open mode.
     * @throw IOException if a file already opened.
     * @throw IOException if open is failed.
     */
    virtual void open(const String& filename, eOpenMode m_mode);

    /**
     * @brief Opens a file, if a file is already opened it will be closed.
     * @param filename  File name.
     * @param mode  Open mode.
     * @throw IOException if a file already opened.
     * @throw IOException if open is failed.
     */
    virtual void open(const char *filename, eOpenMode m_mode);

    /**
     * @brief Checks if a file is already opened.
     */
    virtual bool isOpen();

    /**
     * @brief Closes an opened file..
     * @throw IOException if the the file is already closed or not opened.
     * @throw IOException if can't close the file.
     */
    virtual void close();

    /**
     * @brief Returns true, if the file stream is at the end of file.
     * @return True, if file is end, otherwise return false.
     */
    virtual bool eof();

    /**
     * @brief Writes a boolean on 1 byte to the file.
     * @param b  Output value.
     * @throw IOException if the writing is failed.
     * @throw IOException if the open mode isn't write.
     */
    void writeBool1(bool b);

    /**
     * @brief Reads 1 byte into a bool from the file.
     * @return Readed data.
     * @throw IOException if the reading is failed.
     * @throw IOExceotion if the open mode isn't read.
     */
    bool readBool1();

    /**
     * @brief Writes a charater on 1 byte to the file.
     * @param b  Output value.
     * @throw IOException if the writing is failed.
     * @throw IOExceotion if the open mode isn't write.
     */
    void writeByte1(char c);

    /**
     * @brief Reads 1 byte into a char from the file.
     * @return Readed data.
     * @throw IOException if the reading is failed.
     * @throw IOExceotion if the open mode isn't read.
     */
    char readByte1();

    /**
     * @brief Writes an unsigned charater on 1 byte to the file.
     * @param b  Output value.
     * @throw IOException if the writing is failed.
     * @throw IOExceotion if the open mode isn't write.
     */
    void writeUByte1(unsigned char c);

    /**
     * @brief Reads 1 byte into an unsigned char from the file.
     * @return Readed data
     * @throw IOException if the reading is failed.
     * @throw IOExceotion if the open mode isn't read.
     */
    unsigned char readUByte1();

    /**
     * @brief Writes an integer on 4 byte to the file.
     * @param i  Output value.
     * @throw IOException if the writing is failed.
     * @throw IOExceotion if the open mode isn't write.
     */
    void writeInt4(int i);

    /**
     * @brief Reads 4 byte into an integer from the file.
     * @return Readed data.
     * @throw IOException if the reading is failed.
     * @throw IOExceotion if the open mode isn't read.
     */
    int readInt4();

    /**
     * @brief Writes an unsigned integer on 4 byte to the file.
     * @param i  Output value.
     * @throw IOException if the writing is failed.
     * @throw IOExceotion if the open mode isn't write.
     */
    void writeUInt4(unsigned i);

    /**
     * @brief Reads 4 byte into an unsigned integer from the file.
     * @return Readed data.
     * @throw IOException if the reading is failed.
     * @throw IOExceotion if the open mode isn't read.
     */
    unsigned readUInt4();

    /**
     * @brief Writes a long long on 8 byte to the file.
     * @param i  Output value.
     * @throw IOException if the writing is failed.
     * @throw IOExceotion if the open mode isn't write.
     */
    void writeLongLong8(long long i);

    /**
     * @brief Reads 8 byte into a long long from the file.
     * @return Readed data.
     * @throw IOException if the reading is failed.
     * @throw IOExceotion if the open mode isn't read.
     */
    long long readLongLong8();

    /**
     * @brief Writes an unsigned long long on 8 byte to the file.
     * @param i  Output value.
     * @throw IOException if the writing is failed.
     * @throw IOExceotion if the open mode isn't write.
     */
    void writeULongLong8(unsigned long long i);

    /**
     * @brief Reads 8 byte into an unsigned long long from the file.
     * @return Readed data.
     * @throw IOException if the reading is failed.
     * @throw IOExceotion if the open mode isn't read.
     */
    unsigned long long readULongLong8();

    /**
     * @brief Writes string to the file. String doesn't have maximum length. String can't contains '\0' character.
     * @throw IOException if the writing is failed.
     * @throw IOExceotion if the open mode isn't write.
     */
    void writeString(const String& s);

    /**
     * @brief Reads string (can't read short string) from the file. Read string until found '\0' character.
     * @return Readed data.
     * @throw IOException if the reading is failed.
     * @throw IOExceotion if the open mode isn't read.
     */
    const String readString();

    /**
     * @brief Writes a void* on 'size' byte to the file.
     * @param data  Output value.
     * @param size  The data length.
     * @throw IOException if the writing is failed.
     * @throw IOExceotion if the open mode isn't write.
     */
    void writeData(const void* data, std::streamsize size);

    /**
     * @brief Reads 'size' byte into a void* from the file.
     * @param data  To the data.
     * @param size  The read length.
     * @throw IOException if the reading is failed.
     * @throw IOExceotion if the open mode isn't read.
     */
    void readData (void* data, std::streamsize size);

private:

    /**
     * @brief Returns true if the file is writable.
     * @return True if the file is writable.
     */
    bool isWritable();

    /**
     * @brief Returns true if the file is readable.
     * @return True if the file is readable.
     */
    bool isReadable();

protected:
    /**
     * @brief Pointer to the IO file.
     */
    std::fstream *m_file;

    /**
     * @brief Open mode.
     */
    eOpenMode m_mode;

};

} /* namespace io */

} /* namespace soda */

#endif /* CBINARYIO_H */
