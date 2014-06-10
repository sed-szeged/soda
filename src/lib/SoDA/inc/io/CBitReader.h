/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: Tamás Gergely <gertom@inf.u-szeged.hu>
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

#ifndef CBITREADER_H
#define CBITREADER_H

#include "io/CBinaryIO.h"

namespace soda { namespace io {

/**
 * @brief The CBitReader class implements a bit reader from input stream.
 */
class CBitReader {
public:

    /**
     * @brief Creates a CBitReader object.
     * @param stream Input stream.
     */
    CBitReader(io::CBinaryIO *stream);

    /**
     * @brief Destroys a CBitReader object.
     */
    ~CBitReader();

    /**
     * @brief Reads 1 bit from the buffer and reads 1 byte from the stream if the buffer is empty.
     * @return 1 bit readed from the buffer.
     */
    inline bool readBit() {
        if(m_shift == 8) {
            m_buffer = m_stream->readByte1();
            m_shift = 0;
        }
        return (m_buffer >> m_shift++) & 1;
    }

    /**
     * @brief Resets the buffer of the bit reader.
     */
    inline void reset() {
        m_shift = 8;
        m_buffer = 0;
    }

private:

    /**
     * @brief Input stream.
     */
    io::CBinaryIO *m_stream;

    /**
     * @brief Buffer position.
     */
    int           m_shift;

    /**
     * @brief Buffer of readable data.
     */
    char          m_buffer;
};

} /* namespace io */

} /* namespace soda */


#endif /* CBITREADER_H */
