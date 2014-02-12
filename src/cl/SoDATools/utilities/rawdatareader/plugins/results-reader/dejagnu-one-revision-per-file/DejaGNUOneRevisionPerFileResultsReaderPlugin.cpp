#include <iostream>

#include "CRawDataReaderPluginManager.h"
#include "DejaGNUOneRevisionPerFileResultsReaderPlugin.h"

namespace soda {

DejaGNUOneRevisionPerFileResultsReaderPlugin::DejaGNUOneRevisionPerFileResultsReaderPlugin()
{

}

DejaGNUOneRevisionPerFileResultsReaderPlugin::~DejaGNUOneRevisionPerFileResultsReaderPlugin()
{

}

std::string DejaGNUOneRevisionPerFileResultsReaderPlugin::getName()
{
    return "dejagnu-one-revision-per-file";
}

std::string DejaGNUOneRevisionPerFileResultsReaderPlugin::getDescription()
{
    return "The results information of a revision in one file and the format is: TODO.";
}

CResultsMatrix* DejaGNUOneRevisionPerFileResultsReaderPlugin::read(const std::string &path)
{
    CResultsMatrix *results = new CResultsMatrix();
    std::cout << "Reading " << path << std::endl;
    return results;
}

extern "C" void registerPlugin(CRawDataReaderPluginManager &manager) {
    manager.addResultsReaderPlugin(new DejaGNUOneRevisionPerFileResultsReaderPlugin());
}

} /* namespace soda */
