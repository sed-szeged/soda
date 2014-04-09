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

#include <iostream>
#include <sstream>

#include "CComputeSelectionMetrics.h"
#include "exception/CException.h"

namespace soda {

CComputeSelectionMetrics::SelectionMetrics::SelectionMetrics():
        nofSelected(0),
        nofFailed(0),
        nofHit(0)
{}

CComputeSelectionMetrics::SelectionMetrics::~SelectionMetrics()
{}

CComputeSelectionMetrics::CComputeSelectionMetrics(CSelectionData* data, ITestSuitePrioritizationPlugin* prio, IntVector* revlist, IntVector* sizelist, int dbg):
        m_data(data),
        m_prioAlg(prio),
        m_revisionList(revlist),
        m_sizeList(sizelist),
        m_numberOfRevisions(revlist->size()),
        m_numberOfSelections(sizelist->size()),
        m_nOfTestCases(data->getCoverage()->getNumOfTestcases()),
        m_perRevisionData(NULL),
        m_sumTestcaseData(NULL),
        e_translation(0),
        d_progress_bar(dbg)
{
    m_perRevisionData    = new SelectionMetrics[m_numberOfRevisions*m_numberOfSelections];
    m_sumTestcaseData    = new SelectionMetrics[m_numberOfSelections];
}

CComputeSelectionMetrics::~CComputeSelectionMetrics()
{
    if (m_perRevisionData) {
        delete[] m_perRevisionData;
        m_perRevisionData = NULL;
        m_numberOfRevisions  = 0;
    }

    if (m_sumTestcaseData) {
        delete[] m_sumTestcaseData;
        m_sumTestcaseData = NULL;
        m_numberOfSelections = 0;
    }

    if (e_translation) {
        std::cerr << "[INFO] " << e_translation << " translation errors were detected during the measurements" << std::endl;
    }
}

void CComputeSelectionMetrics::runMeasurementForOneSelectionSize(RevNumType rev, SelectionMetrics* pdata, int size, SelectionMetrics* sdata)
{
    int prg = 0;
    if (d_progress_bar > 2) {
        std::cerr << ":\b" ;
    }

    IntVector selectedTestcases;
    m_prioAlg->fillSelection(selectedTestcases, size);

    pdata->nofSelected = selectedTestcases.size();
    pdata->nofFailed = m_data->getResults()->getExecutionBitList(rev).count() - m_data->getResults()->getPassedBitList(rev).count();
    pdata->nofHit = 0;

    for (IntVector::iterator tcit = selectedTestcases.begin(); tcit != selectedTestcases.end(); tcit++) {
        if (d_progress_bar > 2) {
            std::cerr << "|/-\\"[prg++%4];
        }

        IndexType tcid;
        try {
            tcid = m_data->translateTestcaseIdFromCoverageToResults(*tcit);
        } catch(CException& e) {
            e_translation++;
            continue;
        }

        if (m_data->getResults()->getExecutionBitList(rev)[tcid] &&
                !(m_data->getResults()->getPassedBitList(rev)[tcid])) {
            pdata->nofHit++;
        }

        if (d_progress_bar > 2) {
            std::cerr << '\b';
        }
    }

    (*sdata) += *pdata;

    if (d_progress_bar > 2) {
        std::cerr << " \b" ;
    }
}

void CComputeSelectionMetrics::runMeasurementForOneRevision(RevNumType rev, SelectionMetrics* pdata)
{
    int prg = 0;
    if (d_progress_bar > 1) {
        std::cerr << ":\b";
    }

    m_prioAlg->reset(rev);

    SelectionMetrics* pidx = pdata;
    SelectionMetrics* sidx = m_sumTestcaseData;

    for (IntVector::iterator sizeit = m_sizeList->begin(); sizeit != m_sizeList->end(); ++sizeit) {
        if (d_progress_bar > 1) {
            std::cerr << "|/-\\"[prg++%4];
        }

        runMeasurementForOneSelectionSize(rev, pidx++, *sizeit, sidx++);

        if (d_progress_bar > 1) {
            std::cerr << '\b';
        }

        // when size is larger than the number of test cases than the measurements will not change
        if (*sizeit >= m_nOfTestCases)
            break;
    }

    if (d_progress_bar > 1) {
        std::cerr << " \b" ;
    }
}

void CComputeSelectionMetrics::runMeasurement()
{
    int prg = 0;
    SelectionMetrics* pidx = m_perRevisionData;

    for (IntVector::iterator revit = m_revisionList->begin(); revit != m_revisionList->end(); ++revit) {
        if (d_progress_bar > 0) {
            std::cerr.width(6);
            std::cerr << prg++;
        }

        runMeasurementForOneRevision(*revit, pidx);
        pidx += m_numberOfSelections;

        if (d_progress_bar > 0) {
            std::cerr << "\b\b\b\b\b\b" ;
        }
    }

    if (d_progress_bar > 0) {
        std::cerr << "      \b\b\b\b\b\b" ;
    }
}

String CComputeSelectionMetrics::getData()
{
    std::stringstream ss;
    ss
        << ";#revs;" << m_numberOfRevisions
        << ";;#fail;" << m_sumTestcaseData->nofFailed
        << ";;" << std::endl;
    ss
        << "Sel.lim;Sel.avg;"
        << "incl. (r);prec. (r);f (r);"
        << "incl. (f);prec. (f);f (f);"
        << std::endl;

    for (size_t sel = 0; sel < m_numberOfSelections; sel++) {
        IndexType avgSelSize = 0;
        double inclusiveness = 0.0;
        double precision = 0.0;
        double f_measure = 0.0;
        SelectionMetrics* ptr = m_perRevisionData + sel;

        for (size_t rev = 0; rev < m_numberOfRevisions; rev++, ptr += m_numberOfSelections) {
            avgSelSize += ptr->nofSelected;
            inclusiveness += ptr->inclusiveness();
            precision += ptr->precision();
        }

        avgSelSize /= m_numberOfRevisions;
        inclusiveness /= m_numberOfRevisions;
        precision /= m_numberOfRevisions;
        f_measure = (precision + inclusiveness > 0.0) ? (2 * (precision * inclusiveness) / (precision + inclusiveness)) : 0.0;
        ss
            << m_sizeList->at(sel) << ';'
            << avgSelSize << ';'
            << std::fixed << inclusiveness << ';'
            << std::fixed << precision << ';'
            << std::fixed << f_measure << ';'
            << std::fixed << m_sumTestcaseData[sel].inclusiveness() << ';'
            << std::fixed << m_sumTestcaseData[sel].precision() << ';'
            << std::fixed << m_sumTestcaseData[sel].f_measure() << ';'
            << std::endl;
    }

    ss << std::endl;

    return ss.str();
}

String CComputeSelectionMetrics::getDetailedData()
{
    std::stringstream ss;
    for (size_t sel = 0; sel < m_numberOfSelections; sel++) {
        ss
            << "============================================================" << std::endl
            << "Selection size; " << m_sizeList->at(sel) << std::endl
            << "Revisions ....; " << m_numberOfRevisions << std::endl
            << "Failed .......; " << m_sumTestcaseData->nofFailed << std::endl
            << " rev.;  sel. ; fail. ;  hit. ; incl. ; prec. ;   f   ;" << std::endl;

        IndexType avgSelSize = 0;
        IndexType avgFail = 0;
        IndexType avgHit = 0;
        double inclusiveness = 0.0;
        double precision = 0.0;
        double f_measure = 0.0;
        SelectionMetrics* ptr = m_perRevisionData + sel;

        for (size_t rev = 0; rev < m_numberOfRevisions; rev++, ptr += m_numberOfSelections) {
            avgSelSize += ptr->nofSelected;
            avgFail += ptr->nofFailed;
            avgHit += ptr->nofHit;
            inclusiveness += ptr->inclusiveness();
            precision += ptr->precision();

            ss
                << m_revisionList->at(rev) << ';'
                << ptr->nofSelected << ';'
                << ptr->nofFailed << ';'
                << ptr->nofHit << ';'
                << std::fixed << ptr->inclusiveness() << ';'
                << std::fixed << ptr->precision() << ';'
                << std::fixed << ptr->f_measure() << ';'
                << std::endl;

        }

        avgSelSize /= m_numberOfRevisions;
        inclusiveness /= m_numberOfRevisions;
        precision /= m_numberOfRevisions;
        f_measure = (precision + inclusiveness > 0.0) ? (2 * (precision * inclusiveness) / (precision + inclusiveness)) : 0.0;

        ss
            << " avg.;"
            << avgSelSize << ';'
            << avgFail << ';'
            << avgHit << ';'
            << std::fixed << inclusiveness << ';'
            << std::fixed << precision << ';'
            << std::fixed << f_measure << ';'
            << std::endl
            << " sum.;;"
            << m_sumTestcaseData[sel].nofFailed << ';'
            << m_sumTestcaseData[sel].nofHit << ';'
            << std::fixed << m_sumTestcaseData[sel].inclusiveness() << ';'
            << std::fixed << m_sumTestcaseData[sel].precision() << ';'
            << std::fixed << m_sumTestcaseData[sel].f_measure() << ';'
            << std::endl;
    }

    ss << std::endl;
    return ss.str();
}

} /* namespace soda */
