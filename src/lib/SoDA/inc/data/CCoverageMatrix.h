/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: László Langó <lango@inf.u-szeged.hu>
 *          Tamás Gergely <gertom@inf.u-szeged.hu>
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

#ifndef CCOVERAGEMATRIX_H
#define CCOVERAGEMATRIX_H

#include "interface/IBitMatrix.h"
#include "interface/IIDManager.h"

namespace soda {

/**
 * @brief The CCoverageMatrix class stores the test case names, code element names and a coverage matrix.
 *          The coverage matrix's rows are the test cases and the columns are the code elements.
 *          If the value is 1 in one of the matrix's elements than that code element
 *          was affected in that test case.
 */
class CCoverageMatrix {
public:

    /**
     * @brief Constructor, creates an empty CCoverageMatrix object.
     */
    CCoverageMatrix();

    /**
     * @brief Constructor, creates a CCoverageMatrix object with specified IIDManagers.
     * @param testcases  IIDManager for id, test case name pairs.
     * @param codeElements  IIDManager for id, code element name pairs.
     */
    CCoverageMatrix(IIDManager* testcases, IIDManager* codeElements);

    /**
     * @brief Constructor, creates a CCoverageMatrix object with specified IIDManagers and IBitMatrix.
     * @param testcases  IIDManager for id, test case name pairs.
     * @param codeElements  IIDManager for id, code element name pairs.
     * @param data  Coverage matrix.
     */
    CCoverageMatrix(IIDManager* testcases, IIDManager* codeElements, IBitMatrix* data);

    /**
     * @brief Destroys a CCoverageMatrix object.
     */
    ~CCoverageMatrix();

    /**
     * @brief Returns a reference to the test cases IIDManager.
     * @return Reference to the test cases IIDManager.
     */
    virtual const IIDManager& getTestcases() const;

    /**
     * @brief Returns the test case names which affected the specified code element.
     * @param codeElementName  Specified code element name.
     * @return Affected test case names.
     */
    virtual StringVector getTestcases(const String& codeElementName) const;

    /**
     * @brief Returns a reference to the code elements IIDManager.
     * @return Reference to the code elements IIDManager.
     */
    virtual const IIDManager& getCodeElements() const;

    /**
     * @brief Returns code element names which affected the specified test case.
     * @param testcaseName  Specified test case name.
     * @return Affected code element names.
     */
    virtual StringVector getCodeElements(const String& testcaseName) const;

    /**
     * @brief Returns the coverage matrix.
     * @return Reference to the coverage matrix.
     */
    virtual const IBitMatrix& getBitMatrix() const;

    /**
     * @brief Returns the number of test cases.
     * @return Number of test cases.
     */
    virtual const IndexType getNumOfTestcases() const;

    /**
     * @brief Returns the number of code elements.
     * @return Number of code elements.
     */
    virtual const IndexType getNumOfCodeElements() const;

    /**
     * @brief Returns true if the specified test case and code element
     *          is covered in the bit matrix.
     * @param testcaseName  Test case name.
     * @param codeElementName  Code element name.
     * @return True if the specified test case and code element is covered in the bit matrix.
     */
    virtual bool getRelation(const String& testcaseName, const String& codeElementName) const;

    /**
     * @brief Sets the relation between a test case and a code element in the bit matrix.
     * @param testcaseName  Test case name.
     * @param codeElementName  Code element name.
     * @param isCovered  True if the code element was affected at the test case.
     */
    virtual void setRelation(const String& testcaseName, const String& codeElementName, const bool isCovered=true);

    /**
     * @brief Sets the relation between a test case and a code element in the bit matrix.
     * @param testcaseIdx  Test case id.
     * @param codeElementIdx  Code element id.
     * @param isCovered  True if the code element was affected at the test case.
     */
    virtual void setRelation(const IndexType testcaseIdx, const IndexType codeElementIdx, const bool isCovered=true);

    /**
     * @brief Sets the relation between a test case and a code element in the bit matrix.
     *          If the id of the test case or code element is not in the IIDManager
     *          than it will add to the IIDManager and it will resize the coverage matrix.
     * @param testcaseName  Test case name.
     * @param codeElementName  Code element name.
     * @param isCovered  True if the code element was affected at the test case.
     */
    virtual void addOrSetRelation(const String& testcaseName, const String& codeElementName, const bool isCovered=true);

    /**
     * @brief Resizes the coverage matrix if the IIDManagers have different sizes.
     */
    virtual void refitMatrixSize();

    /**
     * @brief Adds a specified test case name to the IIDManager if it's not in it.
     *        WARNING: This method does not allocate new rows!
     *                    Use refitMatrixSize()!
     */
    virtual void addTestcaseName(const String&);

    /**
     * @brief Adds a specified code element name to the IIDManager if it's not in it.
     *        WARNING: This method does not allocate new rows!
     *                    Use refitMatrixSize()!
     */
    virtual void addCodeElementName(const String&);

    /**
     * @brief Writes the content of the matrix to the out.
     * @param out
     */
    virtual void save(io::CBinaryIO* out) const;

    /**
     * @brief Writes the content of the matrix in the specified file.
     * @param filename  Name of the output file.
     */
    virtual void save(const char * filename) const;

    /**
     * @brief Writes the content of the matrix in the specified file.
     * @param filename  Name of the output file.
     */
    virtual void save(const String& filename) const;

    /**
     * @brief Loads the content of a matrix from in.
     * @param in
     */
    virtual void load(io::CSoDAio* in);

    /**
     * @brief Loads the content of a matrix from a specified file.
     * @param filename  Name of the input file.
     */
    virtual void load(const char * filename);

    /**
     * @brief Loads the content of a matrix from a specified file.
     * @param filename  Name of the input file.
     */
    virtual void load(const String& filename);

private:

    /**
     * @brief IIDManager for test cases names.
     */
    IIDManager* m_testcases;

    /**
     * @brief IIDManager for code element names.
     */
    IIDManager* m_codeElements;

    /**
     * @brief Coverage matrix. Rows stores test cases, columns stores code elements.
     */
    IBitMatrix* m_data;

    /**
     * @brief If true than the constructor creates m_testcases.
     */
    bool m_createTestcases;

    /**
     * @brief If true than the constructor creates m_codeElements.
     */
    bool m_createCodeElements;

    /**
     * @brief If true than the constructor creates m_data.
     */
    bool m_createBitMatrix;

private:

    /**
     * @brief NIY Copy constructor.
     */
    CCoverageMatrix(const CCoverageMatrix&);

    /**
     * @brief NIY operator =-
     * @return Reference to a CCoverageMatrix object.
     */
    CCoverageMatrix& operator=(const CCoverageMatrix&);
};

} // namespace soda

#endif /* CCOVERAGEMATRIX_H */
