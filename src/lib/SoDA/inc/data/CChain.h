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
#include "data/CIDManager.h"
#include "interface/IIterators.h"
#include "interface/IChain.h"
#include "io/CJsonReader.h"

#ifndef CCHAIN_H
#define CCHAIN_H

using namespace std;

namespace soda
{
    /**
     * @brief The CChain class stores the Chain datastructure
     */
    class CChain : public IChain
    {
        private:    
            
            /**
             * @brief SoDA container of elements
             */
            IIDManager* m_codeElements;

            /**
             * @brief The container of ordered elements.
             */
            vector<String>* m_order;
        public:

            /**
             * @brief Constructor
             */
            CChain();

            /**
             * @brief Destructor
             */
            ~CChain();

            /**
             * @brief Gets the count of elements
             * @return count
             */
            IndexType count();

            /**
             * @brief Clears the containers
             */
            virtual void clear();

            /**
             * @brief Adds a new element
             * @param n The new value of the element 
             */
            virtual void add(const String& n);

            /**
             * @brief Adds a vector of new elements to the end of the chain
             * @param codeElements The vector of elements
             */
            virtual void add(const StringVector& codeElements);

            /**
             * @brief Removes a value from the containers
             * @param n The removable value
             */
            virtual void remove(const String& n);

            /**
             * @brief Does the chain contain the element.
             * @param n The new value of the element 
             * @return the answer
             */
            virtual bool contains(const String& n);

            /**
             * @brief Gets the elementId of the value n
             * @param n The requested value
             * @return the index of the element
             */
            virtual IndexType getId(const String& n);

            /**
             * @brief Gets the value of the elementId n
             * @param n The requested id
             * @return the value
             */
            virtual String getValue(const IndexType n);

            /**
             * @brief Gets the first type iterator to the beginnig of chain
             * @return the begin-iterator
             */
            virtual vector<String>::const_iterator first();

            /**
             * @brief Gets the last type iterator to the end of chain
             * @return the end-iterator
             */
            virtual vector<String>::const_iterator end();

            /**
             * @brief Saves a chain as binary data to the out stream
             * @param [IN] out The stream
             */
            virtual void save(io::CBinaryIO *out) const;

            /**
             * @brief Loads chain by binary data from in stream
             * @param [IN] in The source stream
             */
            virtual void load(io::CSoDAio *in);

            /**
             * @brief Saves the chain to file
             * @param [IN] in The filename
             */
            virtual void save(const char * filename) const;

            /**
             * @brief Loads chain from file
             * @param [IN] filename The filename
             */
            virtual void load(const char * filename);

            /**
             * @brief Loads chain from JSON data file
             * @param [IN] filename The requested id
             */
            virtual void loadJson(const String& path);
    };
}

#endif
