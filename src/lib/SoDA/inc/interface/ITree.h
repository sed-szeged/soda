#include <vector>
#include <string>
#include "data/CIDManager.h"
#include "data/CBitMatrix.h"
#include "interface/IGraph.h"

#ifndef ITREE_H
#define ITREE_H

using namespace std;

namespace soda
{
    class ITree //: public IGraph
    {
        public:
            virtual vector<IndexType>& getDFS() = 0;
            virtual vector<IndexType>& getBFS() = 0;
    };
}

#endif
