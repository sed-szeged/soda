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

#ifndef CRELATIONMATRIX_H
#define CRELATIONMATRIX_H

#include "interface/IBitMatrix.h"
#include "CIDManager.h"

namespace soda {

/**
 * @brief The CRelationMatrix class stores relations between code elements.
 */
class CRelationMatrix {

public:

    /**
     * @brief Constructor, creates an empty CRelationMatrix object.
     */
    CRelationMatrix();

    /**
     * @brief Creates a CRelationMatrix with specified bitmatrix and code element names.
     */
    CRelationMatrix(IBitMatrix*, CIDManager*);

    /**
     * @brief Destroys a CRelationMatrix object.
     */
    ~CRelationMatrix();

    /**
     * @brief Sets the relation between two code elements.
     */
    virtual void setRelation(const IndexType, const IndexType, const bool);

    /**
     * @brief Returns the relation between two specified code elements.
     * @return The relation between two code elements
     */
    virtual bool getRelation(const IndexType, const IndexType) const;

    /**
     * @brief Returns the relation between two specified code elements.
     * @return The relation between two code elements.
     */
    virtual bool getRelation(const String&, const String&) const;

    /**
     * @brief Returns the impact set of a specified code element as string.
     * @param codeElementName  Specified code element.
     * @return The code element names affected by the specified code element..
     */
    virtual std::set<String> getImpactSet(const String& codeElementName);

    /**
     * @brief Returns a reference to the IDManager.
     * @return A reference to the IDManager.
     */
    virtual CIDManager& getIdManager() const;

    /**
     * @brief Returns a reference to the relation matrix.
     * @return A reference to the relation matrix.
     */
    virtual const IBitMatrix& getRelationBitMatrix() const;

    /**
     * @brief Returns the size of the relation matrix.
     * @return Size of the relation matrix.
     */
    virtual IndexType getSize() const;

    /**
     * @brief Returns a specified row of the relation matrix as a bitlist.
     * @return Specified row of the relation matrix.
     */
    virtual IBitList& operator[](const IndexType) const;

    /**
     * @brief Returns a specified row of the relation matrix as a bitlist.
     * @return Specified row of the relation matrix.
     */
    virtual IBitList& operator[](const String&) const;

    /**
     * @brief Computes the impact size of the relation matrix.
     * @return Impact size of the relation matrix.
     */
    virtual std::vector<IndexType> computeImpactSetSizes() const;

    /**
     * @brief Computes a detailed message about the relation matrix.
     * @return Detailed message about the relation matrix.
     */
    virtual std::vector<int> computeDetailedMsg() const;

    /**
     * @brief Writes the content of a relation matrix to the out.
     * @param out
     */
    virtual void save(io::CBinaryIO* out) const;

    /**
     * @brief Writes the content of a relation matrix to a specified file.
     * @param filename  Specified file.
     */
    virtual void save(const char * filename) const;

    /**
     * @brief Writes the content of a relation matrix to a specified file
     * @param filename  Specified file.
     */
    virtual void save(const String& filename) const;

    /**
     * @brief Loads the content of a relation matrix from in.
     * @param in
     */
    virtual void load(io::CSoDAio* in);

    /**
     * @brief Loads the content of a relation matrix from a specified file.
     * @param filename  Specified file.
     */
    virtual void load(const char * filename);

    /**
     * @brief Loads the content of a relation matrix from a specified file.
     * @param filename  Specified file.
     */
    virtual void load(const String& filename);

protected:

    /**
     * @brief Stores relation between code elements.
     */
    IBitMatrix* m_relations;

    /**
     * @brief Stores code element id,name pairs.
     */
    CIDManager* m_idManager;

    /**
     * @brief If true than the constructor creates the relation matrix.
     */
    bool m_createRelations;

    /**
     * @brief If true than the constructor creates the IDManager.
     */
    bool m_createIdManager;

private:

    /**
     * @brief NIY copy constructor.
     */
    CRelationMatrix(const CRelationMatrix&);

    /**
     * @brief NIY operator =.
     * @return Reference to a CRelationMatrix object.
     */
    CRelationMatrix& operator=(const CRelationMatrix&);
};

}

#endif /* CRELATIONMATRIX_H */
