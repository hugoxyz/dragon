//
//  MemoryList.cpp
//  dragon
//
//  Created by hugo on 16/5/24.
//
//

#include <stdlib.h>

#include "MemoryList.hpp"
#include "Logger.hpp"

namespace dragon {
    MemoryList::MemoryList()
    : capacity(0)
    , length(0)
    , unitSize(1)
    , memory(nullptr) {
    }
    
    MemoryList::~MemoryList() {
        FREEIF(memory);
        capacity = 0;
        length = 0;
        unitSize = 0;
    }
    
    void MemoryList::setUnitSize(int s) {
        unitSize = s;
    }
    
    void MemoryList::addMemory(void *p) {
        if (nullptr == memory) {
            capacity = 2;
            memory = malloc(capacity * unitSize);
            if (nullptr == memory) {
                LOGD("MemoryList", "malloc fail");
                return;
            }
            memset(memory, 0, capacity * unitSize);
        }
        if (length == capacity) {
            capacity = capacity * 2;
            memory = realloc(memory, capacity * unitSize);
            if (nullptr == memory) {
                LOGD("MemeoryList", "malloc fail");
                return;
            }
            memset(memory, 0, capacity * unitSize);
        }
        memcpy((char*)memory + length * unitSize, p, unitSize);
        length += 1;
    }
    
    void* MemoryList::getMemory(int pos) {
        if (nullptr == memory) {
            return nullptr;
        }
        if (pos >= length) {
            return nullptr;
        }
        return (char*)memory + pos * unitSize;
    }
    
}