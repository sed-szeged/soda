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

#include "exception/CException.h"
#include "CChangesDataManager.h"
#include "CDataHandler.h"
#include <fstream>

namespace sodatools {

CChangesDataManager::CChangesDataManager() : CDataManager()
{}

CChangesDataManager::CChangesDataManager(CDataHandler *handler) :
    CDataManager(handler)
{}

CChangesDataManager::~CChangesDataManager()
{}

void CChangesDataManager::load(const String &filepath)
{
    INFO(getPrintInfo(), "CChangesDataManager::load(\"" << filepath << "\")");
    path p(filepath);
    if (CDataManager::checkPathOk(p) == 1) {
        if (getDataHandler()->getSelection())
            getDataHandler()->getSelection()->loadChangeset(filepath.c_str());
        else
            getDataHandler()->getChanges(true)->load(filepath);
    } else
        throw new CException("CChangesDataManager::load", filepath + " is not a regular file");
}

void CChangesDataManager::save(const String &filepath)
{
    INFO(getPrintInfo(), "CChangesDataManager::save(\"" << filepath << "\")");
    if (getDataHandler()->getSelection())
        getDataHandler()->getSelection()->getChangeset()->save(filepath);
    else if (getDataHandler()->getChanges())
        getDataHandler()->getChanges()->save(filepath);
    else
        WARN("There are no changes data to be saved.");
}

void CChangesDataManager::dumpCodeElements(const String &filepath)
{
    INFO(getPrintInfo(), "CChangesDataManager::dumpCodeElements(\"" << filepath << "\")");
    if (getDataHandler()->getChanges() || getDataHandler()->getSelection()) {
        ofstream O(filepath.c_str());
        const IIDManager& idm = (getDataHandler()->getSelection() ? getDataHandler()->getSelection()->getChangeset() : getDataHandler()->getChanges())->getCodeElements();
        for(IndexType idx = 0; idx < idm.size(); ++idx) {
            O << idx << ':' << idm[idx] << std::endl;
        }
        O.close();
    } else {
        WARN("There are no Changes data to be dumped.");
    }
}

} // namespace sodatools
