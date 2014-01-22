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

#include "io/CBinaryIO.h"
#include "data/CRelationMatrix.h"
#include "data/CBitMatrix.h"
#include "exception/CException.h"

namespace soda {

CRelationMatrix::CRelationMatrix() :
    m_relations(new CBitMatrix()),
    m_idManager(new CIDManager()),
    m_createRelations(true),
    m_createIdManager(true)
{ }

CRelationMatrix::CRelationMatrix(IBitMatrix* bitMatrix, CIDManager* idManager) :
    m_relations(bitMatrix),
    m_idManager(idManager),
    m_createRelations(false),
    m_createIdManager(false)
{ }

CRelationMatrix::~CRelationMatrix()
{
    if(m_createRelations)
        delete m_relations;

    if(m_createIdManager)
        delete m_idManager;
}

void CRelationMatrix::setRelation(const IndexType codeElementIndex1, const IndexType codeElementIndex2, const bool value)
{
    m_relations->set(codeElementIndex1, codeElementIndex2, value);
}

bool CRelationMatrix::getRelation(const IndexType codeElementIndex1, const IndexType codeElementIndex2) const
{
    return m_relations->get(codeElementIndex1, codeElementIndex2);
}

bool CRelationMatrix::getRelation(const String& codeElement1, const String& codeElement2) const
{
    return m_relations->get(m_idManager->getID(codeElement1), m_idManager->getID(codeElement2));
}

std::set<String> CRelationMatrix::getImpactSet(const String& codeElementName)
{
    std::set<String> tmp;

    for(IndexType i = 0; i < m_relations->getNumOfCols(); i++){
        if ((*this)[codeElementName][i])
            tmp.insert((*m_idManager)[i]);
    }
    return tmp;
}

CIDManager& CRelationMatrix::getIdManager() const
{
    return *m_idManager;
}

IndexType CRelationMatrix::getSize() const
{
    return m_relations->getNumOfRows();
}

const IBitMatrix& CRelationMatrix::getRelationBitMatrix() const
{
    return *m_relations;
}

IBitList& CRelationMatrix::operator[](const IndexType row) const
{
    return m_relations->getRow(row);
}

IBitList& CRelationMatrix::operator[](const String& codeElementName) const
{
    return m_relations->getRow((*m_idManager)[codeElementName]);
}

std::vector<IndexType> CRelationMatrix::computeImpactSetSizes() const
{
    std::vector<IndexType> impactSizeList(m_relations->getNumOfRows()+1);

    for(IndexType i = 0 ; i < getSize(); i++ ) {
        IndexType sum = 0;
        for(IndexType j = 0 ; j < m_relations->getRow(i).size(); j++) {
            if(m_relations->get(i,j)) {
                sum++;
            }
        }
        impactSizeList[m_relations->getNumOfRows() - sum]++;
    }

    return impactSizeList;
}

std::vector<int> CRelationMatrix::computeDetailedMsg() const
{
    IndexType ROWS = m_relations->getNumOfRows();
    std::vector<int> resultList;
    std::vector<int> sumVector(ROWS); // number of 1 elements per row
    std::vector<int> hashVector(ROWS); // hash vector of 1 elements per row
    std::vector<int> counterVector(ROWS);
    const int intsize = 8*sizeof(int);

    for(IndexType codeElementIndex = 0 ; codeElementIndex < ROWS; codeElementIndex++ ) {
        for(IndexType j = 0 ; j < ROWS; j++) {
            if(m_relations->get(codeElementIndex, j)) {
                sumVector[codeElementIndex]++;
                hashVector[codeElementIndex] ^= (1 << (j%intsize));
            }
        }
        counterVector[codeElementIndex]++;
        for(IndexType k = 0; k < codeElementIndex; k++) {
            if(sumVector[codeElementIndex] == sumVector[k] && hashVector[codeElementIndex] == hashVector[k] && m_relations->getRow(codeElementIndex) == m_relations->getRow(k)) {
                counterVector[k]++;
                counterVector[codeElementIndex] = 0;
                break;
            }
        }
    }

    for(int sum = ROWS; sum >= 0; --sum) {
        int inv = 1;
        bool zero = true;
        for(IndexType j = 0; j < ROWS; ++j) {
            if(sumVector[j] == sum && counterVector[j] != 0) {
                zero = false;
                int out = (counterVector[j]) * inv;
                resultList.push_back(out);
                inv = -1;
            }
        }
        if(zero) {
            resultList.push_back(0);
        }
    }

    return resultList;

}

void CRelationMatrix::save(io::CBinaryIO* out) const
{
    m_idManager->save(out, io::CSoDAio::PRLIST);
    m_relations->save(out, io::CSoDAio::RELATION);
}

void CRelationMatrix::load(io::CSoDAio* in)
{
    bool isPR = false;
    bool isREL = false;

    while(in->nextChunkID()) {
        if(in->getChunkID() == io::CSoDAio::PRLIST) {
            m_idManager->load(in);
            isPR = true;
        } else if(in->getChunkID() == io::CSoDAio::RELATION) {
            m_relations->load(in);
            isREL = true;
        }
    }

    if(!isPR || !isREL) {
        throw CException("soda::CRelationMatrix","There is not RelationMatrix in this file!");
    }
}


void CRelationMatrix::save(const char * filename) const
{
    io::CSoDAio *out;
    out = new io::CSoDAio(filename, io::CBinaryIO::omWrite);
    save(out);
    out->close();
}

void CRelationMatrix::load(const char * filename)
{
    io::CSoDAio *in;
    in = new io::CSoDAio(filename, io::CBinaryIO::omRead);
    load(in);
    in->close();
}

void CRelationMatrix::save(const String& filename) const
{
    save(filename.c_str());
}

void CRelationMatrix::load(const String& filename)
{
    load(filename.c_str());
}

} // namespace soda
