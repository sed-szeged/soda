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

            if (getDataHandler()->getRevision()) {
                O << csep << "Pass/fail";
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

            // pass fail info for a specified revision
            if (getDataHandler()->getRevision()) {
                CResultsMatrix::TestResultType resType;
                if (getDataHandler()->getSelection()->getResults()->getTestcases().containsValue(coverage->getTestcases().getValue(tcidx))) {
                    resType = getDataHandler()->getSelection()->getResults()->getResult(getDataHandler()->getRevision(), coverage->getTestcases().getValue(tcidx));
                }
                else {
                    std::cout << "Not existing TC: " << coverage->getTestcases().getValue(tcidx) << std::endl;
                    resType = (CResultsMatrix::TestResultType)-1;
                }

                O << csep;
                switch (resType) {
                    case CResultsMatrix::trtNotExecuted:
                        break;
                    case CResultsMatrix::trtFailed:
                        O << 0;
                        break;
                    case CResultsMatrix::trtPassed:
                        O << 1;
                        break;
                    default: // not existing testcase
                        O << -1;
                        break;
                }
            }
            O << rsep;
        }

        // bug data
        if (getDataHandler()->getRevision()) {
            auto bugs = getDataHandler()->getSelection()->getBugs()->getBuggedCodeElements(getDataHandler()->getRevisionTimestamp());
            O << "Bugged";
            for (IndexType ceidx = 0; ceidx < m.getNumOfCols(); ++ceidx) {
                auto ceName = coverage->getCodeElements().getValue(ceidx);
                O << csep << (std::find(bugs.begin(), bugs.end(), ceName) != bugs.end() ? '1' : '0');
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


void CCoverageDataManager::dumpImage(const String &filepath, int scale)
{
    ofstream pgm((filepath + ".pgm").c_str());
    IndexType R = getDataHandler()->getCoverage()->getNumOfTestcases();
    IndexType C = getDataHandler()->getCoverage()->getNumOfCodeElements();
    int granularity = int(C) / scale;
    int width = (granularity > 0) ? granularity : C;
    int height = (width * R + C - 1) / C;
    const unsigned long long int max = ((R + height - 1) / height) * ((C + width - 1) / width);
    const unsigned long long int imax = ((max < 65535) ? max : 65535);
    unsigned int *pic = new unsigned int[width];
    int lastor = 0;

    pgm << "P2" << std::endl << width << " " << height << std::endl << imax << std::endl;
    for (int i = 0; i < width; pic[i++] = 0);
    for (IndexType r = 0; r < R; r++) {
        int nextor = (r * height / R);
        if (nextor != lastor) {
            for (int i = 0; i < width; pic[i++] = 0) {
                pgm << (max - pic[i]) * imax / max << ' ';
            }
            pgm << std::endl;
        }
        lastor = nextor;
        for (IndexType c = 0; c < C; c++) {
            if (getDataHandler()->getCoverage()->getBitMatrix().get(r, c)) {
                pic[c * width / C]++;
            }
        }
    }
    for (int i = 0; i < width; pic[i++] = 0) {
        pgm << (max - pic[i]) * imax / max << ' ';
    }
    pgm << std::endl;
    delete[] pic;
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

void CCoverageDataManager::dumpCodeElementCoverage(const String &filepath) {
    INFO(getPrintInfo(), "CCoverageDataManager::dumpCodeElementCoverage(\"" << filepath << "\")");
    if (!getDataHandler()->getCoverage() && !getDataHandler()->getSelection()) {
        return;
    }
    auto coverage = getDataHandler()->getSelection() ? getDataHandler()->getSelection()->getCoverage() : getDataHandler()->getCoverage();
    ofstream O((filepath + ".csv").c_str());
    O << ";coverage;coverage(%)" << std::endl;

    IndexType nrOfTests = coverage->getNumOfTestcases();
    IntVector coveredCEs;
    coverage->getBitMatrix().colCounts(coveredCEs);
    for (IndexType ceId = 0; ceId < coverage->getNumOfCodeElements(); ++ceId) {
        O << coverage->getCodeElements().getValue(ceId) << ";" << coveredCEs[ceId] << ";" << (double)coveredCEs[ceId] / nrOfTests << std::endl;
    }
}

} // namespace soda
