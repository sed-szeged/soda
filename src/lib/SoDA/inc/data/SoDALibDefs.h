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

#ifndef SODALIBDEFS_H
#define SODALIBDEFS_H

#include <map>
#include <string>
#include <set>
#include <vector>
#include <list>
#include <sys/types.h>


namespace soda {

class CBitList;
class CClusterDefinition;

#ifdef _WIN32
	typedef unsigned long long u_int64_t;
#endif

typedef u_int64_t IndexType;
typedef unsigned int RevNumType;

typedef CBitList BitList;
typedef std::string String;
typedef std::vector<IndexType> IntVector;
typedef std::vector<String> StringVector;
typedef std::map<IndexType, String> StringMap;
typedef std::map<String, IndexType> IdxStrMap;
typedef std::map<IndexType, IndexType> IdxIdxMap;
typedef std::map<std::string, CClusterDefinition> ClusterMap;

} // namespace soda

#endif /* SODALIBDEFS_H */
