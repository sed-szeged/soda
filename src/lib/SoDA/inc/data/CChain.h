#include <vector>
#include <string>
#include "data/CIDManager.h"
#include "interface/IIterators.h"

#ifndef CCHAIN_H
#define CCHAIN_H

using namespace std;

namespace soda
{
    class CChain{

        private:
            IIDManager* m_codeElements;
            vector<String>* m_order;
        public:
            CChain();
            ~CChain();
            IndexType count();
            void clear();
            void add(const String& n);
            void add(const StringVector& codeElements);
            void add(const CChain& codeElements);
            void remove(const String& n);
            bool contains(const String& n);
            IndexType getId(const String& n);
            String getValue(const IndexType n);
            vector<String>::const_iterator first();
            vector<String>::const_iterator end();
            // vector<String>::const_iterator next(const String& n);
            // vector<String>::const_iterator prev(const String& n);
            void save(io::CBinaryIO *out) const;
            void load(io::CSoDAio *in);
            void save(const char * filename) const;
            void load(const char * filename);
    };
}

#endif
