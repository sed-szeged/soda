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

#ifndef IOEXCEPTION_H
#define IOEXCEPTION_H

#include <exception>

#include "data/SoDALibDefs.h"

namespace soda { namespace io {

/**
 * @brief Throwed when a problem occurs during an IO operation.
 */
class CIOException : public std::exception
{
public:
    CIOException(const String& location, const String& message) throw();
    virtual ~CIOException() throw();
    virtual const char* what() const throw();
private:
    String message;
};

} /* namespace io */

} /* namespace soda */

#endif /* IOEXCEPTION_H */
