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

ResultsScoreMetricPlugin::ResultsScoreMetricPlugin() : data(nullptr), tcidList(nullptr) {
}

ResultsScoreMetricPlugin::~ResultsScoreMetricPlugin() {
}

std::string ResultsScoreMetricPlugin::getName() {
    return "results-score";
}

std::string ResultsScoreMetricPlugin::getDescription() {
    return "Calculating results score of the results matrix. Compares the revisions of the results matrix to the base revision (0).";
}

void ResultsScoreMetricPlugin::init(CSelectionData *selection, const rapidjson::Document& args, IntVector *idList) {
    data = selection;
    tcidList = idList;
}

std::vector<std::string> ResultsScoreMetricPlugin::getDependency() {
    return std::vector<std::string>();
}

void ResultsScoreMetricPlugin::calculate(rapidjson::Document &results) {
    // TODO: Unify the data structure of the json files and extract them to constant variables.
    // used constants
    const char* MUTATION_METRICS_TAG = "mutation-metrics";

    CResultsMatrix *tcResults = data->getResults();
    if (!results.HasMember(MUTATION_METRICS_TAG)) {
        results.AddMember(rapidjson::StringRef(MUTATION_METRICS_TAG), rapidjson::Value(rapidjson::kObjectType), results.GetAllocator());
    }

    IndexType nrOfCases = tcResults->getNumOfRevisions() - 1; // number of revisions except the base revision which is the 0 revision number
    std::set<IndexType> newFailRevs;
    std::set<IndexType> newPassRevs;

    IntVector idList;
    if (tcidList == NULL) {
        idList = tcResults->getTestcases().getIDList();
    } else {
        idList = *tcidList;
    }

    for (auto &tcIdx : idList) {
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

    rapidjson::Value::MemberIterator passIt = results[MUTATION_METRICS_TAG].FindMember("pass-results-score");
    if (passIt == results[MUTATION_METRICS_TAG].MemberEnd()) {
        rapidjson::Value v;
        v.SetDouble(((double)newPassRevs.size() / nrOfCases));
        results[MUTATION_METRICS_TAG].AddMember("pass-results-score", v, results.GetAllocator());
    } else
        passIt->value.SetDouble(((double)newPassRevs.size() / nrOfCases));

    rapidjson::Value::MemberIterator failIt = results[MUTATION_METRICS_TAG].FindMember("fail-results-score");
    if (failIt == results[MUTATION_METRICS_TAG].MemberEnd()) {
        rapidjson::Value v;
        v.SetDouble(((double)newFailRevs.size() / nrOfCases));
        results[MUTATION_METRICS_TAG].AddMember("fail-results-score", v, results.GetAllocator());
    }
    else
        failIt->value.SetDouble(((double)newFailRevs.size() / nrOfCases));
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getMutationMetricPluginManager().addPlugin(new ResultsScoreMetricPlugin());
}

} /* namespace soda */
