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
#include <boost/algorithm/string.hpp>

#include "exception/CException.h"
#include "MutationCategoryMetricPlugin.h"

namespace soda {

MutationCategoryMetricPlugin::MutationCategoryMetricPlugin() : data(nullptr),
    mutationCoverage(nullptr), tcidList(nullptr) {
}

MutationCategoryMetricPlugin::~MutationCategoryMetricPlugin() {
}

std::string MutationCategoryMetricPlugin::getName() {
    return "mutation-category";
}

std::string MutationCategoryMetricPlugin::getDescription() {
    return "Calculates the number of mutation for each category types: \n\t1: The enclosing method of the mutation point is not covered."\
            "\n\t2: The enclosing method is covered but the mutation point isn't."\
            "\n\t3: The mutation is covered but it had no effect on the test results."\
            "\n\t4 A: The mutation is covered and at least one test case turned from failed to pass."\
            "\n\t4 B: The mutation is covered and at least one test case turned from pass to failed."\
            "\n\nExcepts additional json line parameters: mutation-coverage which is the coverage matrix containing the mutations"\
            "and mutation-map which contains the revision mutation point mapping.";
}

std::vector<std::string> MutationCategoryMetricPlugin::getDependency() {
    return std::vector<std::string>();
}

void MutationCategoryMetricPlugin::init(CSelectionData *selection, const rapidjson::Document& args, IntVector *idList) {
    data = selection;
    tcidList = idList;

    const char* MUTATION_COVERAGE_PATH = "mutation-coverage";
    if (!args.HasMember(MUTATION_COVERAGE_PATH) || !boost::filesystem::exists(args[MUTATION_COVERAGE_PATH].GetString())) {
        throw CException("MutationCategoryMetricPlugin::init", "Not existing or invalid mutation-coverage path.");
    }
    loadMutationCoverage(args[MUTATION_COVERAGE_PATH].GetString());


    const char* MUTATION_MAP_PATH = "mutation-map";
    if (!args.HasMember(MUTATION_MAP_PATH) || !boost::filesystem::exists(args[MUTATION_MAP_PATH].GetString())) {
        throw CException("MutationCategoryMetricPlugin::init", "Not existing or invalid mutation-map path.");
    }

    pairRevisionWithCoverage(args[MUTATION_MAP_PATH].GetString());
}

void MutationCategoryMetricPlugin::loadMutationCoverage(const String &path) {
    mutationCoverage = new CCoverageMatrix();
    mutationCoverage->load(path);
}

void MutationCategoryMetricPlugin::pairRevisionWithCoverage(const String &path) {
    // convert all code elements to json
    std::vector<rapidjson::Document*> parsedCE;
    StringVector cEs = mutationCoverage->getCodeElements().getValueList();
    for (int i = 0; i < cEs.size(); ++i) {
        rapidjson::Document* doc = new rapidjson::Document();
        doc->Parse<0>(cEs[i].c_str());
        parsedCE.push_back(doc);
    }

    std::ifstream in(path);
    String line;
    while (std::getline(in, line)) {
        StringVector data;
        boost::split(data, line, boost::is_any_of(";"));
        fs::path p(data[5]);
        RevNumType rev = boost::lexical_cast<RevNumType>(p.filename().string());
        for (int i = 0; i < parsedCE.size(); ++i) {
            auto &ce = *parsedCE[i];
            if (ce["mutation"]["type"].GetString() == data[1] &&
                ce["mutation"]["count"].GetInt() == boost::lexical_cast<RevNumType>(data[2]) &&
                ce["file"]["relative_path"].GetString() == data[0]) {
                rev2Coverage[rev] = mutationCoverage->isCoveredCodeElement(cEs[i]);
                break;
            }
        }
    }

    for (int i = 0; i < parsedCE.size(); ++i) {
        delete parsedCE[i];
    }
    parsedCE.clear();
}

void MutationCategoryMetricPlugin::calculate(rapidjson::Document &results) {
    // TODO: Unify the data structure of the json files and extract them to constant variables.
    // used constants
    const char* MUTATION_METRICS_TAG = "mutation-metrics";
    CResultsMatrix *tcResults = data->getResults();

    {
        auto resIt = results.FindMember(MUTATION_METRICS_TAG);
        if (resIt == results.MemberEnd()) {
            results.AddMember(rapidjson::StringRef(MUTATION_METRICS_TAG), rapidjson::Value(rapidjson::kObjectType), results.GetAllocator());
        }
    }
    IndexType nrOfCases = tcResults->getNumOfRevisions() - 1; // number of revisions except the base revision which is the 0 revision number
    std::map<String, IndexType> mutationCats { { "type-1", 0 }, { "type-2", 0 }, { "type-3", 0 }, { "type-4A", 0 }, { "type-4B", 0 } };

    enum ResultsDifferenceMask{
        NO_DIFF = 0x0,
        NEW_PASS = 0x1,
        NEW_FAIL = 0x2,
        BOTH = 0x4
    };

    // we are calculating the 5 categories here
    for (auto &rev : tcResults->getRevisionNumbers()) {
        if (!rev) { // base results
            continue;
        }

        // category 3 and 4 requires mutation point coverage
        if (!rev2Coverage[rev]) {
            continue;
        }

        ResultsDifferenceMask diffMask = NO_DIFF;
        IntVector idList;
        if (tcidList == NULL) {
            idList = tcResults->getTestcases().getIDList();
        } else {
            idList = *tcidList;
        }

        for (auto &tcIdx : idList) {
            auto baseResult = tcResults->getResult(0, tcIdx);
            auto currResult = tcResults->getResult(rev, tcIdx);
            if (baseResult != currResult) {
                // new fail
                if (currResult == CResultsMatrix::trtFailed && baseResult == CResultsMatrix::trtPassed) {
                    switch(diffMask) {
                    case NO_DIFF:
                        diffMask = NEW_FAIL;
                        break;
                    case NEW_PASS:
                        diffMask = BOTH;
                        break;
                    default:
                        break;
                    }
                }
                // new pass
                else if (currResult == CResultsMatrix::trtPassed && baseResult == CResultsMatrix::trtFailed) {
                    switch(diffMask) {
                    case NO_DIFF:
                        diffMask = NEW_PASS;
                        break;
                    case NEW_FAIL:
                        diffMask = BOTH;
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        switch(diffMask) {
        case NO_DIFF:
            mutationCats["type-3"]++;
            break;
        case NEW_FAIL:
            mutationCats["type-4B"]++;
            break;
        case NEW_PASS:
            mutationCats["type-4A"]++;
            break;
        case BOTH:
            mutationCats["type-4A"]++;
            mutationCats["type-4B"]++;
            break;
        }
    }

    const char *NUMBER_OF_MUTATIONS = "nrOfMutations";
    rapidjson::Value::MemberIterator nrOfMutIt = results[MUTATION_METRICS_TAG].FindMember(NUMBER_OF_MUTATIONS);
    if (nrOfMutIt == results[MUTATION_METRICS_TAG].MemberEnd()) {
        rapidjson::Value v;
        v.SetInt(nrOfCases);
        results[MUTATION_METRICS_TAG].AddMember(rapidjson::StringRef(NUMBER_OF_MUTATIONS), v, results.GetAllocator());
    } else
        nrOfMutIt->value.SetInt(nrOfCases);

    for (auto &elem : mutationCats) {
        rapidjson::Value::MemberIterator catIt = results[MUTATION_METRICS_TAG].FindMember(elem.first.c_str());
        if (catIt == results[MUTATION_METRICS_TAG].MemberEnd()) {
            rapidjson::Value v;
            v.SetInt64(elem.second);
            results[MUTATION_METRICS_TAG].AddMember(rapidjson::Value(elem.first.c_str(), results.GetAllocator()), v, results.GetAllocator());
        }
        else
            catIt->value.SetInt(elem.second);
    }

    if (mutationCoverage) {
        delete mutationCoverage;
    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getMutationMetricPluginManager().addPlugin(new MutationCategoryMetricPlugin());
}

} /* namespace soda */
