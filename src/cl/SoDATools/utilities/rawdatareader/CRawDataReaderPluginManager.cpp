#include "CRawDataReaderPluginManager.h"

namespace soda {

CRawDataReaderPluginManager::CRawDataReaderPluginManager() :
    m_changesetReaderPluginManager(new CInternalPluginManager<CRawDataReaderPluginManager>("plugins/changeset-reader")),
    m_changesetReaderPlugins(new std::map<std::string, IChangesetReaderPlugin*>()),
    m_coverageReaderPluginManager(new CInternalPluginManager<CRawDataReaderPluginManager>("plugins/coverage-reader")),
    m_coverageReaderPlugins(new std::map<std::string, ICoverageReaderPlugin*>()),
    m_resultsReaderPluginManager(new CInternalPluginManager<CRawDataReaderPluginManager>("plugins/results-reader")),
    m_resultsReaderPlugins(new std::map<std::string, IResultsReaderPlugin*>())
{}

CRawDataReaderPluginManager::~CRawDataReaderPluginManager()
{
    delete m_changesetReaderPluginManager;
    delete m_changesetReaderPlugins;
    delete m_coverageReaderPluginManager;
    delete m_coverageReaderPlugins;
    delete m_resultsReaderPluginManager;
    delete m_resultsReaderPlugins;
}

void CRawDataReaderPluginManager::loadChangesetReaderPlugins()
{
    m_changesetReaderPluginManager->loadPlugins(*this);
}

void CRawDataReaderPluginManager::addChangesetReaderPlugin(IChangesetReaderPlugin *plugin)
{
    (*m_changesetReaderPlugins)[plugin->getName()] = plugin;
}

IChangesetReaderPlugin* CRawDataReaderPluginManager::getChangesetReaderPlugin(const std::string &name)
{
    return m_changesetReaderPlugins->at(name);
}

std::vector<std::string> CRawDataReaderPluginManager::getChangesetReaderPluginNames()
{
    std::vector<std::string> names;

    for (std::map<std::string, IChangesetReaderPlugin*>::iterator it = m_changesetReaderPlugins->begin();
         it != m_changesetReaderPlugins->end(); it++) {
        names.push_back(it->first);
    }

    return names;
}

void CRawDataReaderPluginManager::loadCoverageReaderPlugins()
{
    m_coverageReaderPluginManager->loadPlugins(*this);
}

void CRawDataReaderPluginManager::addCoverageReaderPlugin(ICoverageReaderPlugin *plugin)
{
    (*m_coverageReaderPlugins)[plugin->getName()] = plugin;
}

ICoverageReaderPlugin* CRawDataReaderPluginManager::getCoverageReaderPlugin(const std::string &name)
{
    return m_coverageReaderPlugins->at(name);
}

std::vector<std::string> CRawDataReaderPluginManager::getCoverageReaderPluginNames()
{
    std::vector<std::string> names;

    for (std::map<std::string, ICoverageReaderPlugin*>::iterator it = m_coverageReaderPlugins->begin();
         it != m_coverageReaderPlugins->end(); it++) {
        names.push_back(it->first);
    }

    return names;

}

void CRawDataReaderPluginManager::loadResultsReaderPlugins()
{
    m_resultsReaderPluginManager->loadPlugins(*this);
}

void CRawDataReaderPluginManager::addResultsReaderPlugin(IResultsReaderPlugin *plugin)
{
    (*m_resultsReaderPlugins)[plugin->getName()] = plugin;
}

IResultsReaderPlugin* CRawDataReaderPluginManager::getResultsReaderPlugin(const std::string &name)
{
    return m_resultsReaderPlugins->at(name);
}

std::vector<std::string> CRawDataReaderPluginManager::getResultsReaderPluginNames()
{
    std::vector<std::string> names;

    for (std::map<std::string, IResultsReaderPlugin*>::iterator it = m_resultsReaderPlugins->begin();
         it != m_resultsReaderPlugins->end(); it++) {
        names.push_back(it->first);
    }

    return names;

}

} /* namespace soda */
