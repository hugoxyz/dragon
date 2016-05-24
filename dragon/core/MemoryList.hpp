//
//  MemoryList.hpp
//  dragon
//
//  Created by hugo on 16/5/24.
//
//

#ifndef MemoryList_hpp
#define MemoryList_hpp

#include "Object.hpp"

namespace dragon {
    class MemoryList : public Object {
    public:

        MemoryList();
        ~MemoryList();

        void setUnitSize(int s);
        void addMemory(void *p);
        void* getMemory(int pos = 0);
        
    protected:
        void* memory;
        int unitSize;
        int capacity;
        int length;
        int step;
    };
}

#endif /* MemoryList_hpp */
