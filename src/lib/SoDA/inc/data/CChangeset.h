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

#ifndef CCHANGESET_H
#define CCHANGESET_H

#include "interface/IBitList.h"
#include "interface/IIDManager.h"
#include "data/CRevision.h"

namespace soda {

/**
 * @brief The CChangeset class stores which code elements were changed in multiple revisions.
 */
class CChangeset {
public:

    /**
     * @brief Constructor, creates an empty CChangeset object.
     */
    CChangeset();

    /**
     * @brief Creates a CChangeset object with specified code elements.
     * @param codeElements  Specified code elements.
     */
    CChangeset(IIDManager* codeElements);

    /**
     * @brief Creates a CChangeset object with specified code elements and change set.
     * @param codeElements  Specified code elements.
     * @param changes  Specified change set.
     */
    CChangeset(IIDManager* codeElements, CRevision<IBitList*>* changes);

    /**
     * @brief Destroys a CChangeset object.
     */
    virtual ~CChangeset();

    /**
     * @brief Returns the change set of a specified revision number.
     * @param revisionNumber  Revision number.
     * @return Code elements changed in the specified revision.
     */
    IBitList& operator[](const RevNumType revisionNumber) const;

    /**
     * @brief Returns the change set of a specified revision number.
     * @param revisionNumber  Revision number.
     * @return Code elements changed in the specified revision.
     */
    IBitList& at(const RevNumType revisionNumber) const;

    /**
     * @brief Returns true if the specified revision exists.
     * @param revisionNumber  Specified revision.
     * @return True if the specified revision exists.
     */
    bool exists(const RevNumType revisionNumber) const;

    /**
     * @brief Returns true if the specified code element was changed in the specified revision.
     * @param revisionNumber  Specified revision.
     * @param codeElementName  Specified code element name.
     * @return True if the specified code element was changed in the specified revision.
     */
    bool isChanged(const RevNumType revisionNumber, const String& codeElementName) const;

    /**
     * @brief Returns the code elements of a specified revision.
     * @param revisionNumber  Specified revision.
     * @return The code elements of a specified revision.
     */
    StringVector getCodeElementNames(const RevNumType revisionNumber) const;

    /**
     * @brief Returns a reference to the code element object.
     * @return A reference to the code elements.
     */
    const IIDManager& getCodeElements() const;

    /**
     * @brief Returns revision numbers where the specified code element was changed.
     * @param codeElementName  Specified code element.
     * @return Revision numbers where the specified code element was changed.
     */
    IntVector getRevisions(const String& codeElementName) const;

    /**
     * @brief Returns the revision numbers.
     * @return Revision numbers.
     */
    IntVector getRevisions() const;

    /**
     * @brief Set the value of the specified code element name in the specified revision to the opposite.
     * @param revisionNumber  Specified revision number.
     * @param codeElementName  Specified code element name.
     */
    virtual void toggle(const RevNumType revisionNumber, const String& codeElementName);

    /**
     * @brief Set the given value to the specified code element in the specified revision.
     * @param revisionNumber  Specified revision.
     * @param codeElementName  Specified code element.
     * @param isChanged  Value to be set.
     */
    virtual void setChange(const RevNumType revisionNumber, const String& codeElementName, const bool isChanged=true);

    /**
     * @brief Adds or sets a code element with a specified revision number and state.
     * @param revisionNumber  Revision number.
     * @param codeElementName  Code element name.
     * @param isChanged  Value to be set.
     */
    virtual void addOrSetChange(const RevNumType revisionNumber, const String& codeElementName, const bool isChanged=true);

    /**
     * @brief Adds a revision with specified number of code elements to the change set.
     * @param revisionNumber  Revision number.
     * @param numberOfCodeElements  Number of code elements.
     */
    virtual void addRevision(const RevNumType revisionNumber, const IndexType numberOfCodeElements);

    /**
     * @brief Adds a specified revision number to the change set.
     * @param revisionNumber  Revision number.
     */
    virtual void addRevision(const RevNumType revisionNumber);

    /**
     * @brief Adds multiple revision numbers to the change set.
     * @param revisionNumbers  Specified revision numbers.
     */
    virtual void addRevision(const IntVector& revisionNumbers);

    /**
     * @brief Adds a new code element to the change set.
     * @param codeElementName  Specified code element name.
     */
    virtual void addCodeElement(const String& codeElementName);

    /**
     * @brief Adds code elements from another code element IDManager.
     * @param codeElements  Code elements to be added.
     */
    virtual void addCodeElement(const IIDManager& codeElements);

    /**
     * @brief Adds multiple code elements to the change set.
     * @param codeElements  List of code element names.
     */
    virtual void addCodeElement(const StringVector& codeElements);

    /**
     * @brief Removes the specified revision from the array.
     * @param revisionNumber  Specified revision number.
     */
    virtual void removeRevision(const RevNumType revisionNumber);

    /**
     * @brief Removes the specified revisions from the array.
     * @param revisionNumbers  Multiple revision numbers.
     */
    virtual void removeRevision(const IntVector& revisionNumbers);

    /**
     * @brief Removes a specified code element from the IDManager and the change set.
     * @param codeElementName  Specified code element.
     */
    virtual void removeCodeElement(const String& codeElementName);

    /**
     * @brief Resizes the change set if the number of code elements is different.
     */
    virtual void refitSize();

    /**
     * @brief Adds a new code element to the IDManager if it's not in it.
     *      WARNING: This method does not allocate new cols!
     *                  Use refitSize()!
     */
    virtual void addCodeElementName(const String&);

    /**
     * @brief Writes the content of a CChangeset object to the out.
     * @param out  Output stream.
     */
    virtual void save(io::CBinaryIO* out) const;

    /**
     * @brief Writes the content of a CChangeset object to a specified file.
     * @param filename  Specified file.
     */
    virtual void save(const char * filename) const;

    /**
     * @brief Writes the content of a CChangeset object to a specified file.
     * @param filename  Specified file.
     */
    virtual void save(const String& filename) const;

    /**
     * @brief Loads the content of a CChangeset object from in.
     * @param in  Input stream.
     */
    virtual void load(io::CSoDAio* in);

    /**
     * @brief Loads the content of a CChangeset object from a specified file.
     * @param filename  Specified file.
     */
    virtual void load(const char * filename);

    /**
     * @brief Loads the content of a CChangeset object from a specified file.
     * @param filename  Specified file.
     */
    virtual void load(const String& filename);

private:

    /**
     * @brief Writes the content of a revision table to the out.
     * @param out  Output stream.
     * @param chunk  Type of the data.
     */
    virtual void saveRevisionTable(io::CBinaryIO* out, const io::CSoDAio::ChunkID chunk) const;

    /**
     * @brief Loads the content of a revision table from in.
     * @param in  Input stream.
     */
    virtual void loadRevisionTable(io::CBinaryIO* in);

private:

    /**
     * @brief Stores code element id,name pairs.
     */
    IIDManager* m_codeElements;


    /**
     * @brief Stores which code elements were changed in a given revision.
     */
    CRevision<IBitList*>* m_changes;

    /**
     * @brief If true than the constructor creates m_codeElements.
     */
    bool m_createCodeElements;

    /**
     * @brief If true than the constructor creates m_changes.
     */
    bool m_createChanges;
};

}

#endif /* CCHANGESET_H */
