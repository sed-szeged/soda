/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: Tamás Gergely <gertom@inf.u-szeged.hu>
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

#ifndef CSELECTIONDATA_H
#define CSELECTIONDATA_H

#include "data/CIDManager.h"
#include "data/CIDMapper.h"
#include "data/CChangeset.h"
#include "data/CCoverageMatrix.h"
#include "data/CResultsMatrix.h"

namespace soda {

/**
 * @brief The CSelectionData class stores compatible coverage, changeset and results data together.
 */
class CSelectionData {
    friend class CSelectionDataAlgorithms;
public:

    /**
     * @brief Constructor, creates an empty CSelectionData object.
     */
    CSelectionData();

    /**
     * @brief Destroys a CSelectionData object.
     */
    virtual ~CSelectionData();

    /**
     * @brief Loads the content of a changeset data from the specified path.
     */
    virtual void loadChangeset(const char*);

    /**
     * @brief Loads the content of a coverage data from the specified path.
     */
    virtual void loadCoverage(const char*);

    /**
     * @brief Loads the content of a results data from the specified path.
     */
    virtual void loadResults(const char*);

    /**
     * @brief Returns a changeset object.
     * @return Changeset object.
     */
    inline CChangeset*      getChangeset() {return m_changeset;}

    /**
     * @brief Returns a coverage object.
     * @return Coverage object.
     */
    inline CCoverageMatrix* getCoverage() {return m_coverage;}

    /**
     * @brief Returns a results object.
     * @return Results object.
     */
    inline CResultsMatrix*  getResults() {return m_results;}

    /**
     * @brief Returns the IDManager of global code element names.
     * @return IDManager of global code element names.
     */
    inline CIDManager* getCodeElements() {return m_globalCodeElements;}

    /**
     * @brief Returns the IDManager of global test case names.
     * @return IDManager of global test case names.
     */
    inline CIDManager* getTestcases() {return m_globalTestcases;}

    /**
     * @brief Translates specified changeset code element id to coverage code element id.
     * @param localFID  Changeset code element id.
     * @return Translated coverage code element id.
     */
    inline IndexType translateCodeElementIdFromChangesetToCoverage(IndexType localFID) {
        return m_coverageCodeElements->translateFromAnotherId(*m_changesetCodeElements, localFID);
    }

    /**
     * @brief Translates specified coverage code element id to changeset code element id.
     * @param localFID  Coverage code element id.
     * @return Translated changeset code element id.
     */
    inline IndexType translateCodeElementIdFromCoverageToChangeset(IndexType localFID) {
        return m_changesetCodeElements->translateFromAnotherId(*m_coverageCodeElements, localFID);
    }

    /**
     * @brief Translates specified results test case id to coverage test case id.
     * @param localTID  Results test case id.
     * @return Translated coverage test case id.
     */
    inline IndexType translateTestcaseIdFromResultsToCoverage(IndexType localTID) {
        return m_coverageTestcases->translateFromAnotherId(*m_resultsTestcases, localTID);
    }

    /**
     * @brief Translates specified coverage test case id to results test case id.
     * @param localTID  Coverage test case id.
     * @return Translated results test case id.
     */
    inline IndexType translateTestcaseIdFromCoverageToResults(IndexType localTID) {
        return m_resultsTestcases->translateFromAnotherId(*m_coverageTestcases, localTID);
    }

    /**
     * @brief Translates a specified test case name to a coverage test case id.
     * @param name  Test case name to be translated.
     * @return Translated coverage test case id.
     */
    inline IndexType translateTestcaseNameToIdInCoverage(const String& name) {
        return m_coverageTestcases->getID(name);
    }

    /**
     * @brief Adds global code element names and test case names into local IDManager.
     */
    virtual void globalize();

protected:

    /**
     * @brief Stores id,name pairs of global code elements.
     */
    CIDManager      *m_globalCodeElements;

    /**
     * @brief Stores id,name pairs of global test cases.
     */
    CIDManager      *m_globalTestcases;

    /**
     * @brief Stores id,name pairs of changeset code elements.
     */
    CIDMapper       *m_changesetCodeElements;

    /**
     * @brief Stores id,name pairs of coverage code elements.
     */
    CIDMapper       *m_coverageCodeElements;

    /**
     * @brief Stores id,name pairs of coverage test cases.
     */
    CIDMapper       *m_coverageTestcases;

    /**
     * @brief Stores id,name pairs of results test cases.
     */
    CIDMapper       *m_resultsTestcases;

    /**
     * @brief Stores changeset data.
     */
    CChangeset      *m_changeset;

    /**
     * @brief Stores coverage data.
     */
    CCoverageMatrix *m_coverage;

    /**
     * @brief Stores results data.
     */
    CResultsMatrix  *m_results;
};

} /* namespace soda*/

#endif /* CSELECTIONDATA_H */
