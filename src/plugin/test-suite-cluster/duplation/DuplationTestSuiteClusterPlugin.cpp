#include "DuplationTestSuiteClusterPlugin.h"

namespace soda {

DuplationTestSuiteClusterPlugin::DuplationTestSuiteClusterPlugin() :
    m_sizes(std::vector<IndexType>())
{
}

DuplationTestSuiteClusterPlugin::~DuplationTestSuiteClusterPlugin()
{
}

std::string DuplationTestSuiteClusterPlugin::getName()
{
    return "duplation";
}

std::string DuplationTestSuiteClusterPlugin::getDescription()
{
    return "Orders the test cases by how well they differentiate the test suite and creates the groups based on this order with the given size.";
}

void DuplationTestSuiteClusterPlugin::init(rapidjson::Document &doc)
{
    m_sizes.clear();
    for (rapidjson::Value::ConstValueIterator it = doc["cluster-sizes"].Begin(); it != doc["cluster-sizes"].End(); it++) {
        m_sizes.push_back(it->GetInt());
    }
}

void DuplationTestSuiteClusterPlugin::execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList)
{
    std::set<IndexType> Trem;  // remaining test set
    std::vector<IndexType> Tv;    // actual test set
    IndexType nrOfTestCases = data.getCoverage()->getNumOfTestcases();
    IndexType nrOfCodeElements = data.getCoverage()->getNumOfCodeElements();

    // initialize the remaining set
    std::set<IndexType>::iterator it = Trem.begin();
    for(IndexType i = 0; i < nrOfTestCases; i++) {
            it = Trem.insert(it, i);
    }

    Tv.clear();

    /* ----- algo start ----- */

    std::vector<unsigned int> R(nrOfCodeElements,1); // stores the classification of methods: in each iteration there are 2^i different values
    std::vector<unsigned int> RCount; // counter for R for finding optimal division: in each iteration its size is 2^i and stores the counts for each category
    std::map<IndexType, std::vector<unsigned int> >  divCount;  // stores 2^i counts for each test case
    unsigned int numClasses = 1;

    // First find the test that best separates the code elements into two groups which are nearly equal size.
    IndexType selectedTest = *Trem.begin();
    double coverOptimal = nrOfCodeElements / 2.0;
    unsigned int coverActual;
    unsigned int coverBest = nrOfCodeElements * 2 + 1; // sufficiently large
    for (it = Trem.begin(); it != Trem.end(); it++) {
        coverActual = data.getCoverage()->getBitMatrix().getRow(*it).count();
        if (std::abs(coverOptimal - coverActual) < std::abs(coverOptimal - coverBest)) {
            coverBest = coverActual;
            selectedTest = *it;
        }
    }

    // mark test case:
    Tv.push_back(selectedTest);
    Trem.erase(selectedTest);
    // update R:
    RCount.clear();
    RCount.resize(numClasses * 2, 0); // for the next iteration
    for (size_t i = 0; i < nrOfCodeElements; i++) {
        R[i] = data.getCoverage()->getBitMatrix().getRow(selectedTest)[i] ? R[i]*2 : R[i]*2-1;
        RCount[R[i] - 1]++;
    }

    while (!Trem.empty()) {
        numClasses *= 2;
        divCount.clear();

        std::vector<IndexType> selectedTestVec(numClasses, 0);
        std::vector<unsigned int> coverBestVec(numClasses, nrOfCodeElements * 2 + 1); // sufficiently large init value
        std::set<IndexType> testOccupied; // not to select the best test case for multiple classes
        for (it = Trem.begin(); it != Trem.end(); it++) {
            IBitList& actRow = data.getCoverage()->getBitMatrix().getRow(*it);
            divCount[*it] = std::vector<unsigned int>(numClasses, 0);
            for (IndexType i = 0; i < nrOfCodeElements; i++)
                if (actRow.at(i))
                    divCount[*it][R[i] - 1]++;
        }

        unsigned int maxc = numClasses - 1;
        for (size_t c = 0; c < numClasses; c++) {
            bool haveIt = false;
            for (it = Trem.begin(); it != Trem.end(); it++) {
                if (std::abs(RCount[c] / 2.0 - divCount[*it][c]) < std::abs(RCount[c] / 2.0 - coverBestVec[c])
                        && testOccupied.find(*it) == testOccupied.end() ) { // we want exactly i^2 different tests to be selected
                    coverBestVec[c] = divCount[*it][c];
                    selectedTestVec[c] = *it;
                    haveIt=true;
                }
            }
            if (!haveIt) { // could not add test case for this class -> finished
                maxc = c - 1;
                break;
            }
            if (!testOccupied.insert(selectedTestVec[c]).second) // oops
                std::cerr << "[DUPLATION] twice occupied test OCCUP: [" << c << "] " << selectedTestVec[c] << std::endl;
            if (std::find(Tv.begin(), Tv.end(), selectedTestVec[c]) != Tv.end()) // oops
                std::cerr << "[DUPLATION] twice occupied test TV: [" << c << "] " << selectedTestVec[c] << std::endl;

        }

        // mark test cases:
        for (size_t c = 0; c <= maxc; c++) {
            Tv.push_back(selectedTestVec[c]);
            Trem.erase(selectedTestVec[c]);
            if (Trem.empty()) {
                maxc = c;
                break;
            }
        }

        // update R:
        RCount.clear();
        RCount.resize(numClasses * 2, 0); // for the next iteration
        for (size_t i = 0; i < nrOfCodeElements ; i++) {
            R[i] = data.getCoverage()->getBitMatrix().getRow(selectedTestVec[R[i] - 1])[i] ? R[i] * 2 : R[i] * 2 - 1;
            RCount[R[i] - 1]++;
        }

        if (Tv.size() >= nrOfTestCases) {
            //Full test suite size reached, exiting.
            break;
        }
    }

    /* ----- algo end ----- */

    // Create the groups based on Tv.
    for (std::vector<IndexType>::iterator it = m_sizes.begin(); it != m_sizes.end(); it++) {
        IndexType size = std::floor(nrOfTestCases * ((double)*it / 100));
        if (size > nrOfTestCases) {
            // TODO Handle this error.
            std::cerr << "Invalid size." << std::endl;
            continue;
        }

        std::string clusterName = boost::lexical_cast<std::string>(*it);
        for (IndexType i = 0; i < size; i++) {
            clusterList[clusterName].addTestCase(Tv[i]);
        }
        clusterList[clusterName].addCodeElements(data.getCoverage()->getCodeElements().getIDList());
    }


}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteClusterPluginManager().addPlugin(new DuplationTestSuiteClusterPlugin());
}

} /* namespace soda */
