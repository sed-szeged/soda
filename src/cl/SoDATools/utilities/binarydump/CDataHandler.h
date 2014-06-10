/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Havas <havasd@inf.u-szeged.hu>
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

#ifndef CDATAHANDLER_H
#define CDATAHANDLER_H

#include "data/SoDALibDefs.h"
#include "data/CCoverageMatrix.h"
#include "data/CResultsMatrix.h"
#include "data/CChangeset.h"
#include "data/CIDManager.h"
#include "data/CSelectionData.h"
#include <iostream>

using namespace std;

namespace soda {

class CCoverageDataManager;
class CChangesDataManager;
class CResultsDataManager;

#define GET(IDMANAGER) (((IDMANAGER) != NULL) ? (IDMANAGER) : (IDMANAGER = new CIDManager()))
#define INFO(print, X) if(print) cerr << "[INFO] " << X << endl
#define WARN(X)                  cerr << "[WARNING] " << X << endl
#define ERRO(X)                  cerr << "[*ERROR*] " << X << endl

enum ReadFormat { rfUnknown, rfOneTestPerFile };

/**
 * @brief The CDataHandler class stores containers for multiple data types.
 */
class CDataHandler
{
public:

    /**
     * @brief Constructor, creates an empty CDataHandler object.
     */
    CDataHandler();

    /**
     * @brief Destroys a CDataHandler object.
     */
    ~CDataHandler();

    /**
     * @brief Returns the read format type.
     * @return Read format type.
     */
    ReadFormat getReadFormat() const;

    /**
     * @brief Sets the read format type to a given value.
     * @param format  Read format type.
     */
    void setReadFormat(ReadFormat format);

    /**
     * @brief Returns the value of m_bPrintInfo member.
     * @return Value of m_bPrintInfo member.
     */
    bool getPrintInfo() const;

    /**
     * @brief Sets the value of m_bPrintInfo member.
     * @param set  Value to be set.
     */
    void setPrintInfo(bool set);

    /**
     * @brief Returns the value of m_bWithPassFail member.
     * @return Value of m_bWithPassFail member.
     */
    bool getWithPassFail() const;

    /**
     * @brief Sets the value of m_bWithPassFail member.
     * @param set  Value to be set.
     */
    void setWithPassFail(bool set);

    /**
     * @brief Returns the value of m_bWithNames member.
     * @return Value of m_bWithNames member.
     */
    bool getWithNames() const;

    /**
     * @brief Sets the value of m_bWithNames member.
     * @param set  Value to be set.
     */
    void setWithNames(bool set);

    /**
     * @brief Creates a CSelectionData object.
     * @return True if the creation was succesful.
     */
    bool createSelection();

    /**
     * @brief Returns a CCoverageDataManager object.
     * @return CCoverageDataManager object.
     */
    CCoverageDataManager getCoverageDataMgr();

    /**
     * @brief Returns a CChangesDataManager object.
     * @return CChangesDataManager object.
     */
    CChangesDataManager getChangesDataMgr();

    /**
     * @brief Returns a CResultsDataManager object.
     * @return CResultsDataManager object.
     */
    CResultsDataManager getResultsDataMgr();

    /**
     * @brief Returns a pointer to m_pChanges member.
     * @param create  If true than the method will create a CChangeset object.
     * @return Pointer to m_pChanges member.
     */
    CChangeset* getChanges(bool create = false);

    /**
     * @brief Returns a pointer to m_pCoverage member.
     * @param create  If true than the method will create a CCoverageMatrix object.
     * @return Pointer to m_pCoverage member.
     */
    CCoverageMatrix* getCoverage(bool create = false);

    /**
     * @brief Sets the value of m_pCoverage member.
     * @param coverage  Value to be set.
     */
    void setCoverage(CCoverageMatrix* coverage);

    /**
     * @brief Returns a pointer to m_pResults member.
     * @param create  If true than the method will create a CResultsMatrix object.
     * @return Pointer to m_pResults member.
     */
    CResultsMatrix* getResults(bool create = false);

    /**
     * @brief Sets the value of m_pResults member.
     * @param results  Value to be set.
     */
    void setResults(CResultsMatrix *results);

    /**
     * @brief Returns a pointer to m_pSelection member.
     * @return Pointer to m_pSelection member.
     */
    CSelectionData* getSelection();

    /**
     * @brief Returns a pointer to m_pTestcases member.
     * @return Pointer to m_pTestcases member.
     */
    CIDManager* getTestcases();

    /**
     * @brief Returns a pointer to m_pCodeElements member.
     * @return Pointer to m_pCodeElements member.
     */
    CIDManager* getCodeElements();

private:

    /**
     * @brief If true than the methods can write to standard output.
     */
    bool m_bPrintInfo;

    /**
     * @brief If true than data has pass fail info.
     */
    bool m_bWithPassFail;

    /**
     * @brief If true than writes code element names and test case names.
     */
    bool m_bWithNames;

    /**
     * @brief Stores read format type.
     */
    ReadFormat m_eReadFormat;

    /**
     * @brief Stores change set data.
     */
    CChangeset *m_pChanges;

    /**
     * @brief Stores coverage data.
     */
    CCoverageMatrix *m_pCoverage;

    /**
     * @brief Stores results data.
     */
    CResultsMatrix *m_pResults;

    /**
     * @brief Stores selection data.
     */
    CSelectionData     *m_pSelection;

    /**
     * @brief Stores test case names.
     */
    CIDManager *m_pTestcases;

    /**
     * @brief Stores code element names.
     */
    CIDManager *m_pCodeElements;
};

} // namespace soda

#endif /* CDATAHANDLER_H */
