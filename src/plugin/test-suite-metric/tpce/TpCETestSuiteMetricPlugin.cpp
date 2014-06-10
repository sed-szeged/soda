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

#include "TpCETestSuiteMetricPlugin.h"

namespace soda {

TpCETestSuiteMetricPlugin::TpCETestSuiteMetricPlugin() :
    m_data(NULL),
    m_clusterList(NULL)
{
}

TpCETestSuiteMetricPlugin::~TpCETestSuiteMetricPlugin()
{
}

std::string TpCETestSuiteMetricPlugin::getName()
{
    return "tpce";
}

std::string TpCETestSuiteMetricPlugin::getDescription()
{
    return "The ratio of test cases to code elements.";
}

void TpCETestSuiteMetricPlugin::init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision)
{
    m_data = data;
    m_clusterList = clusterList;
}

std::vector<std::string> TpCETestSuiteMetricPlugin::getDependency()
{
    return std::vector<std::string>();
}

void TpCETestSuiteMetricPlugin::calculate(const std::string &output, std::map<std::string, MetricResults> &results)
{
    std::ofstream out;
    out.open((output + "/tpce.metric.csv").c_str());
    out << "# cluster id;number of testcases in cluster;number of code elements;tpce" << std::endl;


    std::map<std::string, CClusterDefinition>::iterator it;
    for (it = m_clusterList->begin(); it != m_clusterList->end(); it++) {
        IndexType nrOfCodeElements = it->second.getCodeElements().size();
        IndexType nrOfTestcases = it->second.getTestCases().size();

        double tpce = (double)nrOfTestcases / nrOfCodeElements;

        out << it->first << ";" << nrOfTestcases << ";" << nrOfCodeElements << ";" << tpce << std::endl;

        results[it->first]["tpce"] = tpce;
    }

    out.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new TpCETestSuiteMetricPlugin());
}

} /* namespace soda */
