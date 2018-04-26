#include <vector>
#include <string>
#include "data/CIDManager.h"
#include "data/CBitMatrix.h"

#ifndef ICHAIN_H
#define ICHAIN_H

using namespace std;

namespace soda
{
    class IChain
    {
        public:
            virtual IndexType count() = 0;
            virtual void clear() = 0;
            virtual void add(const String& n) = 0;
            virtual void add(const StringVector& codeElements) = 0;
            virtual void remove(const String& n) = 0;
            virtual bool contains(const String& n) = 0;
            virtual IndexType getId(const String& n) = 0;
            virtual String getValue(const IndexType n) = 0;
            virtual vector<String>::const_iterator first() = 0;
            virtual vector<String>::const_iterator end() = 0;
            virtual void save(io::CBinaryIO *out) const = 0;
            virtual void load(io::CSoDAio *in) = 0;
            virtual void save(const char * filename) const = 0;
            virtual void load(const char * filename) = 0;
    };
}

#endif
