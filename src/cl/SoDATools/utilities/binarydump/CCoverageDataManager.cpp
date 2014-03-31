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
#include "CCoverageDataManager.h"
#include "CDataHandler.h"
#include <fstream>

namespace soda {

CCoverageDataManager::CCoverageDataManager(CDataHandler *handler) :
    CDataManager(handler)
{}

CCoverageDataManager::~CCoverageDataManager()
{}

void CCoverageDataManager::load(const String &filepath)
{
    INFO(getPrintInfo(), "CCoverageDataManager::load(\"" << filepath << "\")");
    path p(filepath);
    if (CDataManager::checkPathOk(p) == 1) {
        if (getDataHandler()->getSelection())
            getDataHandler()->getSelection()->loadCoverage(filepath.c_str());
        else
            getDataHandler()->getCoverage(true)->load(filepath);
    } else
        throw CException("CCoverageDataManager::load", filepath + " is not a regular file");
}

void CCoverageDataManager::dumpData(const String &filepath, bool psize, char csep, char rsep)
{
    INFO(getPrintInfo(), "CCoverageDataManager::dumpData(\"" << filepath << "\")");
    if (getDataHandler()->getCoverage() || getDataHandler()->getSelection()) {
        ofstream O((filepath + ".csv").c_str());
        CCoverageMatrix* coverage = getDataHandler()->getSelection() ? getDataHandler()->getSelection()->getCoverage() : getDataHandler()->getCoverage();
        const IBitMatrix& m = coverage->getBitMatrix();

        if (psize) {
            O << m.getNumOfRows() << csep << m.getNumOfCols() << rsep;
        }

        if (getWithNames()) {
            O << csep << coverage->getCodeElements().getValue(0);
            for (IndexType ceidx = 1; ceidx < m.getNumOfCols(); ++ceidx) {
                O << csep << coverage->getCodeElements().getValue(ceidx);
            }
            O << rsep;
        }

        for (IndexType tcidx = 0; tcidx < m.getNumOfRows(); ++tcidx) {
            if (getWithNames()) {
                O << coverage->getTestcases().getValue(tcidx) << csep;
            }
            O << m[tcidx][0];

            for (IndexType ceidx = 1; ceidx < m.getNumOfCols(); ++ceidx) {
                O << csep << (m[tcidx][ceidx] ? '1' : '0');
            }
            O << rsep;
        }
        O.close();
    } else
        WARN("There is no coverage data to be dumped.");
}

void CCoverageDataManager::dumpTestcases(const String &filepath)
{
    INFO(getPrintInfo(), "CCoverageDataManager::dumpTestcases(\"" << filepath << "\")");
    if (getDataHandler()->getCoverage() || getDataHandler()->getSelection()) {
        ofstream O((filepath + ".csv").c_str());
        const IIDManager& idm = (getDataHandler()->getSelection() ? getDataHandler()->getSelection()->getCoverage() : getDataHandler()->getCoverage())->getTestcases();

        for (IndexType idx = 0; idx < idm.size(); ++idx) {
            O << idx << ':' << idm[idx] << std::endl;
        }
        O.close();
    } else
        WARN("There is no coverage data to be dumped.");
}

void CCoverageDataManager::dumpCodeElements(const String &filepath)
{
    INFO(getPrintInfo(), "CCoverageDataManager::dumpCodeElements(\"" << filepath << "\")");
    if (getDataHandler()->getCoverage() || getDataHandler()->getSelection()) {
        ofstream O((filepath + ".csv").c_str());
        const IIDManager& idm = (getDataHandler()->getSelection() ? getDataHandler()->getSelection()->getCoverage() : getDataHandler()->getCoverage())->getCodeElements();

        for (IndexType idx = 0; idx < idm.size(); ++idx) {
            O << idx << ':' << idm[idx] << std::endl;
        }
        O.close();
    } else {
        WARN("There is no coverage data to be dumped.");
    }
}

} // namespace soda
