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

#ifndef CBITWRITER_H
#define CBITWRITER_H

#include "io/CBinaryIO.h"

namespace soda { namespace io {

/**
 * @brief The CBitWriter class implements a bit writer to an output stream.
 */
class CBitWriter {
public:

    /**
     * @brief Creates a CBitWriter object.
     * @param stream Output stream.
     */
    CBitWriter(io::CBinaryIO *stream);

    /**
     * @brief Destroys a CBitWriter object.
     */
    ~CBitWriter();

    /**
     * @brief Predicts the size of the given data.
     * @param s Data length.
     * @return Size of the data.
     */
    inline static unsigned long long predictSize(unsigned long long s) {
        return (s + 7) / 8;
    }

    /**
     * @brief Writes 1 bit to the buffer and flushes it if the buffer is full.
     * @param val Output value.
     */
    inline void writeBit(bool val) {
        m_buffer |= (val << m_shift++);
        if(m_shift == 8) {
            m_stream->writeByte1(m_buffer);
            m_shift = m_buffer = 0;
        }
    }

    /**
     * @brief Writes the content of the buffer to the output stream immediately.
     */
    inline void flush() {
        if(m_shift > 0) {
            m_stream->writeByte1(m_buffer);
        }
        m_shift = m_buffer = 0;
    }

private:

    /**
     * @brief Output stream.
     */
    io::CBinaryIO *m_stream;

    /**
     * @brief Buffer position.
     */
    int           m_shift;

    /**
     * @brief Buffer of writable data.
     */
    char          m_buffer;
};

} /* namespace io */

} /* namespace soda */


#endif /* CBITWRITER_H */
