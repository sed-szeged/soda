/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Author: David Imre Adam <adam.david.imre@gmail.com>
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

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "data/CIDManager.h"
#include "data/CChain.h"
#include "interface/IIterators.h"

using namespace std;
using namespace soda::io;

namespace soda
{
    CChain::CChain():
        m_codeElements(new CIDManager()),
        m_order(new vector<String>())
    {}

    CChain::~CChain()
    {
        delete m_codeElements;
        m_codeElements = NULL;
        delete m_order;
        m_order = NULL;
    }

    IndexType CChain::count()
    {
        return m_codeElements->size();
    }

    void CChain::clear()
    {
        m_codeElements->clear();
        m_order->clear();
    }

    void CChain::add(const String& n)
    {
        m_codeElements->add(n);
        m_order->push_back(n);
    }

    void CChain::add(const StringVector& codeElements)
    {
        std::copy ( codeElements.begin(), codeElements.end(), m_order->end() );
        
        for(StringVector::const_iterator it = codeElements.begin(); it != codeElements.end(); ++it) {
            m_codeElements->add(*it);
        }
    }

    void CChain::remove(const String& n)
    {
        m_codeElements->remove(n);
        m_order->erase(std::remove(m_order->begin(), m_order->end(), n), m_order->end());
    }

    bool CChain::contains(const String& n)
    {
        return m_codeElements->containsValue(n);
    }

    IndexType CChain::getId(const String& n)
    {
        return m_codeElements->getID(n);
    }

    String CChain::getValue(const IndexType n)
    {
        return m_codeElements->getValue(n);
    }

    vector<String>::const_iterator CChain::first()
    {
        return m_order->begin();
    }

    vector<String>::const_iterator CChain::end()
    {
        return m_order->end();
    }

    void CChain::save(io::CBinaryIO *out) const
    {
        IndexType size = m_order->size();
        unsigned long long int length = (1 + size) * sizeof(IndexType);

        //write IDManager
        m_codeElements->save(out, io::CSoDAio::IDMANAGER);
        //write ChunkID
        out->writeUInt4(io::CSoDAio::CHAIN);
        //write length
        out->writeULongLong8(length);
        //write the number of elements
        out->writeLongLong8(size);
        //write elements
        for(vector<string>::const_iterator it = m_order->begin() ; it != m_order->end(); ++it ) {
            out->writeString(*it);
        }
    }

    void CChain::load(io::CSoDAio *in)
    {
        m_codeElements->clear();
        m_order->clear();

        while(in->nextChunkID()) {
            auto chunkId = in->getChunkID();

            if(chunkId == io::CSoDAio::IDMANAGER) {
                m_codeElements->load(in);
            }
            if(chunkId == io::CSoDAio::CHAIN){
                IndexType size = in->readLongLong8();

                for (IndexType i = 0; i < size; ++i) {
                    String value = in->readString();

                    m_order->push_back(value);
                }
            }
        }
    }

    void CChain::save(const char * filename) const
    {
        io::CSoDAio *out = new io::CSoDAio(filename, io::CBinaryIO::omWrite);
        save(out);
        delete out;
    }

    void CChain::load(const char * filename)
    {
        io::CSoDAio *in = new io::CSoDAio(filename, io::CBinaryIO::omRead);
        load(in);
        delete in;
    }

    void CChain::loadJson(const String& path)
    {
        CJsonReader *reader  = new CJsonReader(path);

        //Read IDManager elements
        StringVector strings = reader->getStringVectorFromProperty("chain");
        for(IndexType i = 0; i <  strings.size(); i++)
        {
            String value = strings[i];
            m_codeElements->add(value);
            m_order->push_back(value);
        }

        delete reader;
        reader = 0;
    }
}
