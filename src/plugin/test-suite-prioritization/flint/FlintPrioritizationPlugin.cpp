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

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "FlintPrioritizationPlugin.h"

namespace soda {

bool operator<(FlintPrioritizationPlugin::qelement d1, FlintPrioritizationPlugin::qelement d2) {
    return d1.priorityValue > d2.priorityValue;
}

FlintPrioritizationPlugin::FlintPrioritizationPlugin():
        m_data(NULL),
        m_nofElementsReady(0),
        m_elementsReady(NULL),
        m_elementsRemaining(NULL),
        m_firstPhase(true),
        m_revision(0),
        m_greedyAlgorithm(NULL),
        m_priorityQueue(new std::vector<qelement>())
{}

FlintPrioritizationPlugin::~FlintPrioritizationPlugin()
{
    delete m_priorityQueue;
    delete m_elementsReady;
    delete m_elementsRemaining;
    m_elementsReady = NULL;
    m_nofElementsReady = 0;
}

String FlintPrioritizationPlugin::getName()
{
    return "flint";
}

String FlintPrioritizationPlugin::getDescription()
{
    return "FLINT algorihtm by Yoo, Harman & Clark.";
}

void FlintPrioritizationPlugin::init(CSelectionData *data, CKernel *kernel)
{
    m_kernel = kernel;
    m_data = data;
    m_greedyAlgorithm = kernel->getTestSuitePrioritizationPluginManager().getPlugin("additional-general-ignore");
    m_greedyAlgorithm->init(data, kernel);
    IntVector initial;
    setState(initial);
}

void FlintPrioritizationPlugin::setState(IntVector &ordered)
{
    delete m_elementsReady;
    delete m_elementsRemaining;
    m_elementsReady = new IntVector(ordered);

    m_elementsRemaining = new IntVector();
    IndexType nofTestcases = m_data->getCoverage()->getNumOfTestcases();
    for(IndexType tcid = 0; tcid < nofTestcases; tcid++) {
        if (std::find(m_elementsReady->begin(), m_elementsReady->end(), tcid) == m_elementsReady->end()) {
            m_elementsRemaining->push_back(tcid);
        }
    }

    m_nofElementsReady = ordered.size();
    m_priorityQueue->clear();
    m_firstPhase = false;
    calculateCounters();
}

void FlintPrioritizationPlugin::reset(RevNumType revision)
{
    m_revision = revision;
    return;
}

void FlintPrioritizationPlugin::fillSelection(IntVector& selected, size_t size)
{
    while (m_nofElementsReady < size && !(m_priorityQueue->empty())) {
        next();
    }

    selected.clear();
    for (size_t i = 0; i < size && i < m_nofElementsReady; i++) {
        selected.push_back((*m_elementsReady)[i]);
    }
}

IndexType FlintPrioritizationPlugin::next()
{
    if (m_elementsRemaining->empty()) {
        throw std::out_of_range("There are not any testcases left.");
    }

    IndexType tcid;
    if (m_firstPhase) {
        tcid = m_greedyAlgorithm->next();
    } else {
        prioritize();
        tcid = (m_priorityQueue->back()).testcaseId;
    }

    // Remove the test from the remaining list
    m_elementsRemaining->erase(std::find(m_elementsRemaining->begin(), m_elementsRemaining->end(), tcid));
    m_elementsReady->push_back(tcid);
    m_nofElementsReady++;

    if (m_firstPhase && !(m_data->getResults()->getExecutionBitList(m_revision).at(tcid) && m_data->getResults()->getPassedBitList(m_revision).at(tcid))) {
        // The test failed, switch to second phase
        m_firstPhase = false;
        calculateCounters();
    }

    return tcid;
}

void FlintPrioritizationPlugin::prioritize()
{
    m_priorityQueue->clear();

    for (IndexType i = 0; i < m_elementsRemaining->size(); i++) {
        qelement d;
        d.testcaseId = m_elementsRemaining->at(i);
        d.priorityValue = entropyLookahead(m_elementsRemaining->at(i));
        m_priorityQueue->push_back(d);
    }

    sort(m_priorityQueue->begin(), m_priorityQueue->end());
}

double FlintPrioritizationPlugin::tarantula(IndexType tp, IndexType tf, IndexType cp, IndexType cf)
{
    double rp = (tp == 0) ? 0.0 : cp / rp;
    double rf = (tf == 0) ? 0.0 : cf / tf;


    return rf / (rp + rf);
}

void FlintPrioritizationPlugin::calculateCounters()
{
    IndexType nofCodeElements = m_data->getCoverage()->getNumOfCodeElements();

    m_tpi = 0;
    m_tfi = 0;

    m_cpi.clear();
    m_cfi.clear();
    m_cpi.resize(nofCodeElements, 0);
    m_cfi.resize(nofCodeElements, 0);

    for(IndexType i = 0; i < m_nofElementsReady; i++) {
        updateCounters(m_elementsReady->at(i));
    }


}

void FlintPrioritizationPlugin::updateCounters(IndexType tcid)
{
    IndexType nofCodeElements = m_data->getCoverage()->getNumOfCodeElements();

    bool passed = m_data->getResults()->getExecutionBitList(m_revision).at(tcid) && m_data->getResults()->getPassedBitList(m_revision).at(tcid);
    if (passed) {
        m_tpi++;
    } else {
        m_tfi++;
    }

    for (IndexType cid = 0; cid < nofCodeElements; cid++) {
        if (passed) {
            m_cpi[cid]++;
        } else {
            m_cfi[cid]++;
        }
    }

}

double FlintPrioritizationPlugin::entropyLookahead(IndexType tcid)
{
    IndexType nofCodeElements = m_data->getCoverage()->getNumOfCodeElements();
    std::vector<double> tau_p(nofCodeElements, 0.0);
    std::vector<double> tau_f(nofCodeElements, 0.0);
    std::vector<double> probabilities(nofCodeElements, 0.0);
    double tau_p_sum = 0.0;
    double tau_f_sum = 0.0;
    double probability_sum = 0.0;

    for (IndexType cid = 0; cid < nofCodeElements; cid++) {
        IndexType cp = m_cpi[cid];
        IndexType cf = m_cfi[cid];
        if (m_data->getCoverage()->getBitMatrix().get(tcid, cid)) {
            cp++;
            cf++;
        }
        tau_p[cid] = tarantula(m_tpi + 1, m_tfi, cp, m_cfi[cid]);
        tau_f[cid] = tarantula(m_tpi, m_tfi + 1, m_cpi[cid], cf);

        tau_p_sum += tau_p[cid];
        tau_f_sum += tau_f[cid];
    }


    for (IndexType cid = 0; cid < nofCodeElements; cid++) {
        probabilities[cid] = ((m_tfi/(m_tpi + m_tfi)) * (tau_f[cid]/tau_f_sum)) + (((m_tpi/(m_tpi + m_tfi)) * (tau_p[cid]/tau_p_sum)));
        probability_sum += probabilities[cid];
    }

    double H = 0.0;
    for (IndexType cid = 0; cid < nofCodeElements; cid++) {
        H += -1 * (probabilities[cid]/probability_sum) * log(probabilities[cid]/probability_sum);
    }
    return H;
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuitePrioritizationPluginManager().addPlugin(new FlintPrioritizationPlugin());
}

} /* namespace soda */
