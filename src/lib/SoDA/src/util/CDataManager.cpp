/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Havas <havasd@inf.u-szeged.hu>
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

#include "util/CDataManager.h"
#include "util/CDataHandler.h"

namespace soda {

CDataManager::CDataManager(CDataHandler *handler) :
    m_pDataHandler(handler)
{}

CDataManager::~CDataManager()
{}

CDataHandler* CDataManager::getDataHandler()
{
    return m_pDataHandler;
}

bool CDataManager::getPrintInfo() const
{
    return m_pDataHandler->getPrintInfo();
}

bool CDataManager::getWithPassFail() const
{
    return m_pDataHandler->getWithPassFail();
}

bool CDataManager::getWithNames() const
{
    return m_pDataHandler->getWithNames();
}

ReadFormat CDataManager::getReadFormat() const
{
    return m_pDataHandler->getReadFormat();
}

int CDataManager::checkPathOk(path p)
{
    if (exists(p)) {
        if (is_regular_file(p)) {
            return 1;
        } else if (is_directory(p)) {
            return 2;
        } else {
          ERRO("The specified path " << p << " exists, but is neither a regular file nor a directory");
          return 0;
        }
    }  else {
        ERRO("The specified path " << p << " does not exist");
        return 0;
    }
    return 0;
}

} // namespace soda
