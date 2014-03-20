/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors:
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

#include <algorithm>
#include <cmath>
#include <time.h>
#include <list>

#include "CReductionPluginManager.h"
#include "CoverageReductionPlugin.h"
#include "CReductionData.h"
#include "data/CBitList.h"

namespace soda {

CoverageReductionPlugin::CoverageReductionPlugin() :
    m_data(NULL)
{
}

String CoverageReductionPlugin::getName()
{
    return "coverage";
}

String CoverageReductionPlugin::getDescription()
{
    return "CoverageReductionPlugin is based on coverage information.";
}

void CoverageReductionPlugin::init(CSelectionData *data, CJsonReader &reader)
{
    m_data = data;
    m_programName = reader.getStringFromProperty("program-name");
    m_dirPath = reader.getStringFromProperty("output-dir");
    m_nrOfCodeElements = data->getCoverage()->getNumOfCodeElements();
    m_nrOfTestCases = data->getCoverage()->getNumOfTestcases();
}

void CoverageReductionPlugin::reduction(std::ofstream &outStream)
{
    coverageReduction(outStream);
}

void CoverageReductionPlugin::coverageReduction(std::ofstream &outStream)
{
    std::cerr << "[INFO] Coverage-based reduction (largest first and vector based) ... ";

    std::set<IndexType> Tgen, Tgeniter, Tvec;    // actual test sets (General 'largets first' strategy for full coverage and iterations and Vector-based strategy)
    std::multimap<unsigned int, IndexType> To;    // stores the test cases in the order of their coverage (first:coverage, second:test case id)
    CBitList totCoverage(m_nrOfCodeElements), actCoverage(m_nrOfCodeElements);    // coverage vectors based on matrix rows

    totCoverage.setAll(false);
    actCoverage.setAll(false);

    outStream << "Number of procedures: " << m_nrOfCodeElements << std::endl;

    // initialize the ordered and randomized lists
    unsigned int cov;
    for(unsigned int i = 0; i < m_nrOfTestCases; i++) {
            cov = m_data->getCoverage()->getBitMatrix().getRow(i).count();
            To.insert(std::multimap<unsigned int, IndexType>::value_type(cov, i));
            addCoverage(totCoverage, m_data->getCoverage()->getBitMatrix().getRow(i));
    }
    unsigned int fullSize = To.size();

    // print initial test suites
    outStream << "Total " << fullSize << " test cases in full test suite." << std::endl;
    outStream << "Total coverage: " << totCoverage.count() << " procedures (" << 100.0 * (totCoverage.count()) / m_nrOfCodeElements << "\%)." << std::endl;

    CReductionData reducedMatrix_gen(m_data->getCoverage(), m_programName + "-GEN", m_dirPath);
    CReductionData reducedMatrix_geniter(m_data->getCoverage(), m_programName + "-GENITER", m_dirPath);
    CReductionData reducedMatrix_vec(m_data->getCoverage(), m_programName + "-VEC", m_dirPath);

    /* ----- algo start ----- */

    std::list<IndexType> tcl; // for the vector-based method
    std::list<IndexType>::iterator tclbeg; // iterator for the list
    unsigned int lastcov = 0; // from the previous iteration
    unsigned int iter = 1;     // for the General strategy for all duplation iterations
    unsigned int itersize = 1; // for the General strategy for all duplation iterations

    // traverse all test cases starting from the largest covering one:
    for (std::multimap<unsigned int, IndexType>::reverse_iterator rit = To.rbegin(); rit != To.rend(); rit++) {
        // Vector-based method:
        if (rit->first != lastcov) // new coverage-vector size class
            tclbeg = tcl.insert(tcl.end(), rit->second); // first element of the size class
        else {
            bool doinsert = true;
            for (std::list<IndexType>::iterator it = tclbeg; it != tcl.end(); it++) {
                if (m_data->getCoverage()->getBitMatrix().getRow(*it) == m_data->getCoverage()->getBitMatrix().getRow(rit->second)) {
                    doinsert = false;
                    break;
                }
            }
            if (doinsert)
                tcl.insert(tcl.end(), rit->second);
        }
        lastcov = rit->first;

        // General method:
        if (actCoverage.count() < totCoverage.count() || setSmaller(actCoverage, totCoverage)) {
            addCoverage(actCoverage, m_data->getCoverage()->getBitMatrix().getRow(rit->second));
            Tgen.insert(rit->second);
        }

        // General method for all duplation iterations. In iteration i, total 2^i-1 test cases are in the set:
        Tgeniter.insert(rit->second);
        if (Tgeniter.size() == itersize) { // new iteration size reached
            reducedMatrix_geniter.add(Tgeniter);
            reducedMatrix_geniter.save(iter);
            Tgeniter.clear();
            iter++;        // 1,2,3,...
            itersize *= 2; // 1,2,4,...
        }
    } // endfor

    // General method for all duplation iterations
    if (!Tgeniter.empty()) { // there are some elements left from the last iteration but  itersize was not reached
      reducedMatrix_geniter.add(Tgeniter);
      reducedMatrix_geniter.save(iter);
    }

    // Vector-based method:
    for (std::list<IndexType>::iterator it = tcl.begin(); it!=tcl.end(); it++)
      Tvec.insert(*it);

    /* ----- algo end ----- */

    // save test matrices
    reducedMatrix_gen.add(Tgen);
    reducedMatrix_gen.save(0);
    reducedMatrix_vec.add(Tvec);
    reducedMatrix_vec.save(0);

    unsigned int reducedSize_gen = Tgen.size();
    unsigned int reducedSize_vec = Tvec.size();

    outStream << "Coverage-based reduction ended" << std::endl;

    // print final test suites:
    outStream << std::endl;
    outStream << "Total " << reducedSize_gen << " test cases in reduced test suite (General)." << std::endl;
    outStream << "Reduction rate: " << 100.0 * (fullSize - reducedSize_gen) / fullSize << "\%" << std::endl;
    outStream << "Final coverage (General): " << actCoverage.count() << " procedures (" << 100.0 * (actCoverage.count()) / m_nrOfCodeElements << "\%)." << std::endl;
    outStream << std::endl;
    outStream << "Total " << reducedSize_vec << " test cases in reduced test suite (Vector-based)." << std::endl;
    outStream << "Reduction rate: " << 100.0 * (fullSize - reducedSize_vec) / fullSize << "\%" << std::endl;
    std::cerr << "done." << std::endl;
}

IndexType CoverageReductionPlugin::addCoverage(IBitList& to, const IBitList& from) const
{
  if (to.size() != from.size()) { // oops
    std::cerr << "[COVERAGE] coverage update failed" << std::endl;
    return 0;
  }
  for (IndexType i=0; i<from.size(); i++)
    if (!to.at(i))
      to.set(i,from.at(i));
  return to.size();
}

bool CoverageReductionPlugin::setSmaller(const IBitList& lhs, const IBitList& rhs) const
{
  if (lhs.size() != rhs.size())
    return true;
  for (IndexType i=0; i<lhs.size(); i++)
    if (rhs.at(i) && !lhs.at(i))
      return true;
  return false;
}

extern "C" void registerPlugin(CReductionPluginManager &manager)
{
    manager.addReductionPlugin(new CoverageReductionPlugin());
}

} /* namespace soda */
