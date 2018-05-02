#include <vector>
#include <string>
#include "data/CIDManager.h"
#include "interface/IIterators.h"
#include "interface/IChain.h"

#ifndef CCHAIN_H
#define CCHAIN_H

using namespace std;

namespace soda
{
    class CChain : public IChain
    {
        private:
            IIDManager* m_codeElements;
            vector<String>* m_order;
        public:
            CChain();
            ~CChain();
            IndexType count();
            virtual void clear();
            virtual void add(const String& n);
            virtual void add(const StringVector& codeElements);
            virtual void remove(const String& n);
            virtual bool contains(const String& n);
            virtual IndexType getId(const String& n);
            virtual String getValue(const IndexType n);
            virtual vector<String>::const_iterator first();
            virtual vector<String>::const_iterator end();
            virtual void save(io::CBinaryIO *out) const;
            virtual void load(io::CSoDAio *in);
            virtual void save(const char * filename) const;
            virtual void load(const char * filename);
    };
}

#endif
