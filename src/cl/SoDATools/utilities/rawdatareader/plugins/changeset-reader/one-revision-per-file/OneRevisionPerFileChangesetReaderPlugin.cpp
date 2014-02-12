#include <iostream>

#include "CRawDataReaderPluginManager.h"
#include "OneRevisionPerFileChangesetReaderPlugin.h"

namespace soda {

OneRevisionPerFileChangesetReaderPlugin::OneRevisionPerFileChangesetReaderPlugin()
{

}

OneRevisionPerFileChangesetReaderPlugin::~OneRevisionPerFileChangesetReaderPlugin()
{

}

std::string OneRevisionPerFileChangesetReaderPlugin::getName()
{
    return "one-revision-per-file";
}

std::string OneRevisionPerFileChangesetReaderPlugin::getDescription()
{
    return "The change information is in one file for a revision and the format is: TODO.";
}

CChangeset* OneRevisionPerFileChangesetReaderPlugin::read(const std::string &path)
{
    CChangeset *changeset = new CChangeset();
    std::cout << "Reading " << path << std::endl;
    return NULL;
}

extern "C" void registerPlugin(CRawDataReaderPluginManager &manager) {
    manager.addChangesetReaderPlugin(new OneRevisionPerFileChangesetReaderPlugin());
}

} /* namespace soda */
