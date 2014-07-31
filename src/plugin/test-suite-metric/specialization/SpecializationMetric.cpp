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

#include <fstream>

#include "SpecializationMetric.h"

namespace soda {

SpecializationMetric::SpecializationMetric() :
    m_data(NULL),
    m_clusterList(NULL),
    m_revision(0)
{

}

SpecializationMetric::~SpecializationMetric()
{

}

std::string SpecializationMetric::getName()
{
    return "specialization";
}

std::string SpecializationMetric::getDescription()
{
    return "Calculating specialization metric of the test suite.";
}

void SpecializationMetric::init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision)
{
    m_data = data;
    m_clusterList = clusterList;
    m_revision = revision;
}


std::vector<std::string> SpecializationMetric::getDependency()
{
    return std::vector<std::string>();
}

void SpecializationMetric::calculate(rapidjson::Document &results)
{
    /*
    std::ofstream coverageStream;
    coverageStream.open((output + "/specialization.metric.csv").c_str());
    coverageStream << "# cluster id;number of testcases in cluster;number of code elements;specialization" << std::endl;
    */

    CCoverageMatrix *coverage = m_data->getCoverage();
    IndexType nrOfTestcases = coverage->getNumOfTestcases();

    std::map<IndexType, bool> tcMap;

    std::cout << std::endl;

    for (IndexType i = 0; i < nrOfTestcases; i++) {
        tcMap[i] = m_data->getResults()->getExecutionBitList(m_revision).at(i);
    }


    std::map<std::string, CClusterDefinition>::iterator it;
    for (it = m_clusterList->begin(); it != m_clusterList->end(); it++) {

        if (!results.HasMember(it->first.c_str())) {
            rapidjson::Value key;
            key.SetString(it->first.c_str(), results.GetAllocator());
            rapidjson::Value cluster;
            cluster.SetObject();
            results.AddMember(key, cluster, results.GetAllocator());
        }

        std::cout << "Processing " << it->first << std::endl;
        IndexType nrOfCodeElementsInCluster = it->second.getCodeElements().size();
        IndexType nrOfTestcasesInCluster = it->second.getTestCases().size();

        std::set<IndexType> allTestcases;
        std::set<IndexType> clusterTestCases;
        for (IndexType i = 0; i < nrOfTestcases; i++) {
            if (tcMap[i]) {
                allTestcases.insert(i);
            }
            if (i < nrOfTestcasesInCluster && tcMap[it->second.getTestCases().at(i)]) {
                clusterTestCases.insert(it->second.getTestCases().at(i));
            }
        }

        double specialization = 0.0;
        IndexType coverageInCluster = 0;
        IndexType totalCoverage = 0;

        for (IndexType i = 0; i < nrOfCodeElementsInCluster; i++) {
            IndexType cid = it->second.getCodeElements().at(i);

            // Count coverage in cluster
            std::set<IndexType>::iterator tit = clusterTestCases.begin();
            while (tit != clusterTestCases.end()) {
                std::set<IndexType>::iterator current = tit++;
                IndexType tcid = *current;
                if (coverage->getBitMatrix().get(tcid, cid)) {
                    clusterTestCases.erase(current);
                    coverageInCluster++;
                }
            }
            // Count coverage in the whole test suite
            tit = allTestcases.begin();
            while (tit != allTestcases.end()) {
                std::set<IndexType>::iterator current = tit++;
                IndexType tcid = *current;
                if (coverage->getBitMatrix().get(tcid, cid)) {
                    allTestcases.erase(current);
                    totalCoverage++;
                }
            }
        }
        if (totalCoverage > 0) {
            specialization = (double)coverageInCluster / totalCoverage;
        }

        rapidjson::Value::MemberIterator metricIt = results[it->first.c_str()].FindMember("specialization");
        if (metricIt == results[it->first.c_str()].MemberEnd()) {
            rapidjson::Value v;
            v.SetDouble(specialization);
            results[it->first.c_str()].AddMember("specialization", v, results.GetAllocator());
        } else
            metricIt->value.SetDouble(specialization);

        //coverageStream << it->first << ";" << nrOfTestcasesInCluster << ";" << nrOfCodeElementsInCluster << ";" << specialization  << std::endl;
    }
    //coverageStream.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new SpecializationMetric());
}

} /* namespace soda */
