#include <iostream>

#include "CRawDataReaderPluginManager.h"
#include "OneTestPerFileCoverageReaderPlugin.h"

namespace soda {

OneTestPerFileCoverageReaderPlugin::OneTestPerFileCoverageReaderPlugin()
{

}

OneTestPerFileCoverageReaderPlugin::~OneTestPerFileCoverageReaderPlugin()
{

}

std::string OneTestPerFileCoverageReaderPlugin::getName()
{
    return "one-test-per-file";
}

std::string OneTestPerFileCoverageReaderPlugin::getDescription()
{
    return "The coverage information of a test case is in one file and the file name is the test case name.";
}

CCoverageMatrix* OneTestPerFileCoverageReaderPlugin::read(const std::string &path)
{
    CCoverageMatrix *matrix = new CCoverageMatrix();
    std::cout << "Reading " << path << std::endl;
    return matrix;
}

extern "C" void registerPlugin(CRawDataReaderPluginManager &manager) {
    manager.addCoverageReaderPlugin(new OneTestPerFileCoverageReaderPlugin());
}

} /* namespace soda */
