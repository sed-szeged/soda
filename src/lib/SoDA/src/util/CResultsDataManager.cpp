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
#include "util/CResultsDataManager.h"
#include "util/CDataHandler.h"
#include <fstream>

namespace soda {

CResultsDataManager::CResultsDataManager(CDataHandler *handler) :
    CDataManager(handler)
{}

CResultsDataManager::~CResultsDataManager()
{}

void CResultsDataManager::load(const String &filepath)
{
    INFO(getPrintInfo(), "CResultsDataManager::load(\"" << filepath << "\")");
    path p(filepath);
    if (CDataManager::checkPathOk(p) == 1) {
        if (getDataHandler()->getSelection())
            getDataHandler()->getSelection()->loadResults(filepath.c_str());
        else
            getDataHandler()->getResults(true)->load(filepath);
    } else
        throw CException("CResultsDataManager::load", filepath + " is not a regular file");
}

void CResultsDataManager::dumpTestcases(const String &filepath)
{
    INFO(getPrintInfo(), "CResultsDataManager::dumpTestcases(\"" << filepath << "\")");
    if (getDataHandler()->getResults() || getDataHandler()->getSelection()) {
        std::ofstream O((filepath + ".csv").c_str());
        const IIDManager& idm = (getDataHandler()->getSelection() ? getDataHandler()->getSelection()->getResults() : getDataHandler()->getResults())->getTestcases();

        for (IndexType idx = 0; idx < idm.size(); ++idx) {
            O << idx << ':' << idm[idx] << std::endl;
        }
        O.close();
    } else
        WARN("There is no results data to be dumped.");
}

void CResultsDataManager::dumpRevisions(const String& filepath)
{
    INFO(getPrintInfo(), "CResultsDataManager::dumpRevisions(\"" << filepath << "\")");
    if (getDataHandler()->getResults() || getDataHandler()->getSelection()) {
        std::ofstream O((filepath + ".csv").c_str());
        IntVector revList = (getDataHandler()->getSelection() ? getDataHandler()->getSelection()->getResults() : getDataHandler()->getResults())->getRevisionNumbers();

        for (IndexType idx = 0; idx < revList.size(); ++idx) {
            O << idx << ':' << revList[idx] << std::endl;
        }
        O.close();
    } else {
        WARN("There is no results data to be dumped.");
    }
}

void CResultsDataManager::dumpExecution(const String& filepath, bool psize, char csep, char rsep)
{
    INFO(getPrintInfo(), "CResultsDataManager::dumpExecution(\"" << filepath << "\")");
    if (getDataHandler()->getResults() || getDataHandler()->getSelection()) {
        std::ofstream O((filepath + ".csv").c_str());
        const IBitMatrix& m = (getDataHandler()->getSelection() ? getDataHandler()->getSelection()->getResults() : getDataHandler()->getResults())->getExecutionBitMatrix();

        if (psize) {
            O << m.getNumOfRows() << csep << m.getNumOfCols() << rsep;
        }

        for (IndexType rev = 0; rev < m.getNumOfRows(); ++rev) {
            O << m[rev][0];
            for(IndexType tcidx = 1; tcidx < m.getNumOfCols(); ++tcidx) {
                O << csep << (m[rev][tcidx] ? '1' : '0');
            }
            O << rsep;
        }
        O.close();
    } else {
        WARN("There is no results data to be dumped.");
    }
}

void CResultsDataManager::dumpPassFail(const String& filepath, bool psize, char csep, char rsep)
{
    INFO(getPrintInfo(), "CResultsDataManager::dumpPassFail(\"" << filepath << "\")");
    if (getDataHandler()->getResults() || getDataHandler()->getSelection()) {
        std::ofstream O((filepath + ".csv").c_str());
        CResultsMatrix const& res = *(getDataHandler()->getSelection() ? getDataHandler()->getSelection()->getResults() : getDataHandler()->getResults());
        IBitMatrix const &m = res.getPassedBitMatrix();

        if (psize) {
            O << res.getNumOfRevisions() << csep << res.getNumOfTestcases() << rsep;
        }

        auto tcIdList = res.getTestcases().getIDList();
        if (getWithNames()) {
            for (auto &tcidx : tcIdList) {
                O << csep << res.getTestcases().getValue(tcidx);
            }

            O << csep << "New fail" << csep << "New pass" << rsep;
        }

        for (auto &rev : res.getRevisionNumbers()) {
            IndexType nrOfNewFail = 0, nrOfNewPass = 0;
            if (getWithNames()) {
                O << rev << csep;
            }

            for (auto &tcidx : tcIdList) {
                switch(res.getResult(rev, tcidx)) {
                    case CResultsMatrix::trtPassed:
                        O << '1';
                        if (res.getResult(0, tcidx) == CResultsMatrix::trtFailed) {
                            ++nrOfNewPass;
                        }
                        break;
                    case CResultsMatrix::trtFailed:
                        O << '0';
                        if (res.getResult(0, tcidx) == CResultsMatrix::trtPassed) {
                            ++nrOfNewFail;
                        }
                        break;
                    case CResultsMatrix::trtNotExecuted:
                        O << "-1";
                        break;
                }
                O << csep;
            }

            if (getWithNames()) {
                O << nrOfNewFail << csep << nrOfNewPass;
            }
            O << rsep;
        }
        O.close();
    } else
        WARN("There is no results data to be dumped.");
}

void CResultsDataManager::dumpTimeline(const String& filepath, char csep, char rsep)
{
    INFO(getPrintInfo(), "CResultsDataManager::dumpTimeline(\"" << filepath << "\")");
    if (getDataHandler()->getResults() || getDataHandler()->getSelection()) {
        std::ofstream O((filepath + ".csv").c_str());
        CResultsMatrix* results = getDataHandler()->getSelection() ? getDataHandler()->getSelection()->getResults() : getDataHandler()->getResults();
        IntVector revList = results->getRevisionNumbers();
        const IIDManager& idm = results->getTestcases();

        O << csep;
        for (IndexType idx = 0; idx < revList.size(); ++idx) {
            O << revList[idx] << csep;
        }
        O << rsep;

        for (IndexType idx = 0; idx < idm.size(); ++idx) {
            O << idm[idx] << csep;
            for (IndexType ridx = 0; ridx < revList.size(); ++ridx) {
                if (results->isExecuted(revList[ridx], idm[idx])) {
                    O << results->isPassed(revList[ridx], idm[idx]);
                }
                O << csep;
            }
            O << rsep;
        }


        O.close();
    } else {
        WARN("There is no results data to be dumped.");
    }
}

} // namespace soda
