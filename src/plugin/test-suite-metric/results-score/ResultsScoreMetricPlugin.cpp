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

#include "ResultsScoreMetricPlugin.h"

namespace soda {

ResultsScoreMetricPlugin::ResultsScoreMetricPlugin() : data(nullptr), clusterList(nullptr) {
}

ResultsScoreMetricPlugin::~ResultsScoreMetricPlugin() {
}

std::string ResultsScoreMetricPlugin::getName() {
    return "results-score";
}

std::string ResultsScoreMetricPlugin::getDescription() {
    return "Calculating results score of the results matrix.";
}

void ResultsScoreMetricPlugin::init(CSelectionData *selection, std::map<std::string, CClusterDefinition> *clusters, IndexType revision) {
    data = selection;
}

std::vector<std::string> ResultsScoreMetricPlugin::getDependency() {
    return std::vector<std::string>();
}

void ResultsScoreMetricPlugin::calculate(rapidjson::Document &results) {
    CResultsMatrix *tcResults = data->getResults();
    if (!results.HasMember("results-metrics")) {
        results.AddMember(rapidjson::Value("results-metrics", results.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), results.GetAllocator());
    }

    IndexType nrOfTestcases = tcResults->getNumOfTestcases();
    IndexType nrOfCases = tcResults->getNumOfRevisions() - 1; // number of revisions except the base revision which is the 0 revision number
    std::set<IndexType> newFailRevs;
    std::set<IndexType> newPassRevs;

    for (auto &tcIdx : tcResults->getTestcases().getIDList()) {
        auto baseResult = tcResults->getResult(0, tcIdx);
        for (auto &rev : tcResults->getRevisionNumbers()) {
            if (!rev) { // base results
                continue;
            }
            auto currResult = tcResults->getResult(rev, tcIdx);
            // new fail
            if (currResult == CResultsMatrix::trtFailed && baseResult == CResultsMatrix::trtPassed) {
                newFailRevs.insert(rev);
            }
            // new pass
            if (currResult == CResultsMatrix::trtPassed && baseResult == CResultsMatrix::trtFailed) {
                newPassRevs.insert(rev);
            }
        }
    }

    rapidjson::Value::MemberIterator passIt = results["results-metrics"].FindMember("pass-results-score");
    if (passIt == results["results-metrics"].MemberEnd()) {
        rapidjson::Value v;
        v.SetDouble(((double)newPassRevs.size() / nrOfCases));
        results["results-metrics"].AddMember("pass-results-score", v, results.GetAllocator());
    } else
        passIt->value.SetDouble(((double)newPassRevs.size() / nrOfCases));

    rapidjson::Value::MemberIterator failIt = results["results-metrics"].FindMember("fail-results-score");
    if (failIt == results["results-metrics"].MemberEnd()) {
        rapidjson::Value v;
        v.SetDouble(((double)newFailRevs.size() / nrOfCases));
        results["results-metrics"].AddMember("fail-results-score", v, results.GetAllocator());
    }
    else
        failIt->value.SetDouble(((double)newFailRevs.size() / nrOfCases));
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new ResultsScoreMetricPlugin());
}

} /* namespace soda */
