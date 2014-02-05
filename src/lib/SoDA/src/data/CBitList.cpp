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

#include "data/CBitList.h"
#include "exception/CException.h"
#include <iterator>
#include "interface/IIterators.h"

namespace soda {

/**
 * @brief The CBitList::ListIterator class is an iterator for CBitList.
 */
class CBitList::ListIterator :
        public IBitListIterator,
        public std::iterator<std::input_iterator_tag, bool> {

private:
    std::vector<bool>::iterator p;

public:
    ListIterator() {}
    ListIterator(std::vector<bool>::iterator it) : p(it) {}
    ListIterator(IBitListIterator& it) : p(static_cast<CBitList::ListIterator*>(&it)->p) {}
    ListIterator(const ListIterator& it) : p(it.p) {}

    IBitListIterator& operator++()
    {
        ++p;
        return *this;
    }
    IBitListIterator& operator++(int)
    {
        p++;
        return *this;
    }
    bool operator==(IBitListIterator& rhs)
    {
        return (p == static_cast<CBitList::ListIterator*>(&rhs)->p);
    }
    bool operator!=(IBitListIterator& rhs)
    {
        return (p != static_cast<CBitList::ListIterator*>(&rhs)->p);
    }
    bool operator*()
    {
        return *p;
    }
};

CBitList::CBitList() :
    m_data(new std::vector<bool>(0)),
    m_beginIterator(0),
    m_endIterator(0),
    m_count(0)
{}

CBitList::CBitList(IndexType size) :
    m_beginIterator(0),
    m_endIterator(0),
    m_count(0)
{
    m_data = new std::vector<bool>(size);
}

CBitList::~CBitList()
{
    delete m_data;
    delete m_beginIterator;
    delete m_endIterator;
}

bool CBitList::front() const
{
    if (m_data->size() == 0)
        throw CException("soda::CBitList::front()", "The list is empty!");

    return m_data->front();
}

bool CBitList::back() const
{
    if (m_data->size() == 0)
            throw CException("soda::CBitList::back()", "The list is empty!");

    return m_data->back();
}

bool CBitList::at(IndexType pos) const
{
    if (pos >= m_data->size() || pos < 0)
        throw CException("soda::CBitList::at()", "Index out of bound!");

    return (*m_data)[pos];
}

void CBitList::push_back(bool value){
    m_data->push_back(value);
    if(value) m_count++;
}

void CBitList::set(IndexType pos, bool value)
{
    if (pos >= m_data->size() || pos < 0)
        throw CException("soda::CBitList::set()", "Index out of bound!");

    (*m_data)[pos] = value;
    if(value) m_count++;
}

void CBitList::toggleValue(IndexType pos)
{
    if (pos >= m_data->size() || pos < 0) {
        throw CException("soda::CBitList::toggleValue()", "Index out of bound!");
    } else if ((*m_data)[pos]) {
        (*m_data)[pos] = false;
        m_count--;
    } else {
        (*m_data)[pos] = true;
        m_count++;
    }
}

void CBitList::pop_back()
{
    if(m_data->size() == 0) {
        throw CException("soda::CBitList::pop_back()","The list is empty!");
    }

    if(m_data->back()) m_count--;
    m_data->pop_back();
}

void CBitList::pop_front()
{
    if(m_data->size() == 0) {
        throw CException("soda::CBitList::pop_front()","The list is empty!");
    }

    if(m_data->front()) m_count--;
    for(IndexType i = 1; i < m_data->size(); i++) {
        (*m_data)[i-1] = (*m_data)[i];
    }
    m_data->pop_back();
}

void CBitList::erase(IndexType pos)
{
    if (pos >= m_data->size() || pos < 0)
        throw CException("soda::CBitList::erase()", "Index out of bound!");
    else if (pos == m_data->size()-1) {
        pop_back();
        return;
    }
    if(m_data->at(pos)) m_count--;

    for(IndexType i = pos+1; i < m_data->size(); i++) {
        (*m_data)[i-1] = (*m_data)[i];
    }
    m_data->pop_back();
}

void CBitList::resize(IndexType newSize)
{
    m_data->resize(newSize, false);
}

void CBitList::clear()
{
    m_data->clear();
    m_count = 0;
    delete m_beginIterator;
    m_beginIterator = 0;
    delete m_endIterator;
    m_endIterator = 0;
}

IndexType CBitList::size() const
{
    return m_data->size();
}

IndexType CBitList::count() const
{
    return m_count;
}

bool CBitList::operator[](IndexType pos) const
{
    return (*m_data)[pos];
}

IBitListIterator& CBitList::begin()
{
    delete m_beginIterator;
    m_beginIterator = new CBitList::ListIterator(m_data->begin());

    return *m_beginIterator;
}

IBitListIterator& CBitList::end()
{
    delete m_endIterator;
    m_endIterator = new CBitList::ListIterator(m_data->end());

    return *m_endIterator;
}

} // namespace soda
