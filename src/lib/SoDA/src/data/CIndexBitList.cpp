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

#include <algorithm>
#include <iterator>

#include "data/CIndexBitList.h"
#include "exception/CException.h"
#include "interface/IIterators.h"

namespace soda {

/**
 * @brief The CIndexBitList::ListIterator class is an iterator fo CIndexBitList.
 */
class CIndexBitList::ListIterator :
        public IBitListIterator,
        public std::iterator<std::input_iterator_tag, bool> {

private:
    IndexType p;
    std::vector<IndexType>::iterator e;
    std::vector<IndexType>::iterator i;
public:
    ListIterator() :
        p(0) {}
    ListIterator(IndexType v, std::vector<IndexType> *c) :
        p(v),
        e(c->end()),
        i(c->begin())
    {
        while ((i != e) && (*i < p)) {
            ++i;
        }
    }
    ListIterator(IBitListIterator& it) :
        p(static_cast<CIndexBitList::ListIterator*>(&it)->p),
        e(static_cast<CIndexBitList::ListIterator*>(&it)->e),
        i(static_cast<CIndexBitList::ListIterator*>(&it)->i) {}
    ListIterator(const ListIterator& it) :
        p(it.p),
        e(it.e),
        i(it.i) {}

    IBitListIterator& operator++()
    {
        ++p;
        if ((i != e) && (*i < p)) {
            ++i;
        }
        return *this;
    }

    IBitListIterator& operator++(int)
    {
        //TODO: make it postfix!!
        ++p;
        if((i!=e) && (*i < p)) {
            ++i;
        }
        return *this;
    }

    bool operator==(IBitListIterator& rhs)
    {
        return (p == static_cast<CIndexBitList::ListIterator*>(&rhs)->p) &&
            (e == static_cast<CIndexBitList::ListIterator*>(&rhs)->e) &&
            (i == static_cast<CIndexBitList::ListIterator*>(&rhs)->i);
    }

    bool operator!=(IBitListIterator& rhs)
    {
        return (p != static_cast<CIndexBitList::ListIterator*>(&rhs)->p) ||
            (e != static_cast<CIndexBitList::ListIterator*>(&rhs)->e) ||
            (i != static_cast<CIndexBitList::ListIterator*>(&rhs)->i);
    }

    bool operator*()
    {
        return (i != e) && (*i == p);
    }
};

CIndexBitList::CIndexBitList() :
    m_data(new std::vector<IndexType>()),
    m_size(0),
    m_beginIterator(0),
    m_endIterator(0)
{}

CIndexBitList::CIndexBitList(IndexType size) :
    m_data(new std::vector<IndexType>(/*size*/)),
    m_size(size),
    m_beginIterator(0),
    m_endIterator(0)
{}

CIndexBitList::~CIndexBitList()
{
    delete m_data;
    delete m_beginIterator;
    delete m_endIterator;
}

bool CIndexBitList::front() const
{
    if(m_size == 0)
        throw CException("soda::CIndexBitList::front()","The list is empty!");

    if(m_data->size() == 0) {
        return false;
    } else {
        return m_data->front() == 0;
    }
}
bool CIndexBitList::back() const
{
    if(m_size == 0)
        throw CException("soda::CIndexBitList::back()","The list is empty!");

    if(m_data->size() == 0) {
        return false;
    } else {
        return m_data->back() == m_size-1 ? true : false;
    }
}

bool CIndexBitList::at(IndexType pos) const
{
    if (pos >= m_size || pos < 0)
        throw CException("soda::CIndexBitList::at()", "Index out of bound!");

    return std::find(m_data->begin(), m_data->end(), pos) != m_data->end();
}

void CIndexBitList::push_back(bool value)
{
    if(value) {
        m_data->push_back(m_size);
    }
    m_size++;
}

void CIndexBitList::set(IndexType pos, bool value)
{
    if (pos >= m_size || pos < 0)
        throw CException("soda::CIndexBitList::set()", "Index out of bound!");

    std::vector<IndexType>::iterator it = std::find(m_data->begin(), m_data->end(), pos);
    if( it != m_data->end() && !value) {
        m_data->erase(it);
    } else if (it == m_data->end() && value){
        m_data->push_back(pos);
    }
}

void CIndexBitList::toggleValue(IndexType pos)
{
    if (pos >= m_size || pos < 0)
            throw CException("soda::CIndexBitList::toggleValue()", "Index out of bound!");

    std::vector<IndexType>::iterator it = std::find(m_data->begin(), m_data->end(), pos);
    if( it != m_data->end() ) {
        m_data->erase(it);
    } else {
        m_data->push_back(pos);
    }
}

void CIndexBitList::pop_back()
{
    if(m_size == 0) {
        throw CException("soda::CIndexBitList::pop_back()","The list is empty!");
    }
    m_size--;
    std::vector<IndexType>::iterator it = std::find(m_data->begin(), m_data->end(), m_size);
    if(it != m_data->end()) {
        m_data->erase(it);
    }
}

void CIndexBitList::pop_front()
{
    if(m_size == 0) {
        throw CException("soda::CIndexBitList::pop_front()","The list is empty!");
    }
    m_size--;
    std::vector<IndexType>::iterator it = std::find(m_data->begin(), m_data->end(), 0);
    if(it != m_data->end()) {
        m_data->erase(it);
    }

    for(std::vector<IndexType>::iterator it = m_data->begin(); it != m_data->end(); ++it) {
        (*it)--;
    }
}

void CIndexBitList::erase(IndexType pos)
{
    if (m_size <= pos || pos < 0)
        throw CException("soda::CIndexBitList::erase()", "Index out of bound!");

    if (pos == (m_size - 1))
        pop_back();
    else
        --m_size;

    for(std::vector<IndexType>::iterator it = m_data->begin(); it != m_data->end(); ++it) {
        if(*it == pos) {
            m_data->erase(it);
        } else if(*it > pos) {
            (*it)--;
        }
    }
}

void CIndexBitList::clear()
{
    delete m_data;
    m_data = 0;
    m_size = 0;
    delete m_beginIterator;
    m_beginIterator = 0;
    delete m_endIterator;
    m_endIterator = 0;
}

IndexType CIndexBitList::size() const
{
    return m_size;
}

IndexType CIndexBitList::count() const
{
    return m_data->size();
}
bool CIndexBitList::operator[](IndexType pos) const
{
    return std::find(m_data->begin(), m_data->end(), pos) != m_data->end();
}

IBitListIterator& CIndexBitList::begin()
{
    delete m_beginIterator;
    std::sort(m_data->begin(), m_data->end());
    m_beginIterator = new CIndexBitList::ListIterator(0, m_data);
    return *m_beginIterator;

}

IBitListIterator& CIndexBitList::end()
{
    delete m_endIterator;
    m_endIterator = new CIndexBitList::ListIterator(m_size, m_data);
    return *m_endIterator;
}

void CIndexBitList::resize(IndexType newSize)
{
    std::sort(m_data->begin(), m_data->end());
    if(newSize < m_size) {
        std::vector<IndexType>::iterator it;
        for (it = m_data->begin(); (it != m_data->end()) && (*it < newSize); it++);
        m_data->erase(it, m_data->end());
    }
    m_size = newSize;
}

} // namespace soda
