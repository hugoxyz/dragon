//
//  IBuffer.cpp
//  dragon
//
//  Created by hugo on 16/6/16.
//
//

#include <stdlib.h>
#include <math.h>
#include "Logger.hpp"
#include "IBuffer.hpp"

namespace dragon {
    IBuffer::IBuffer()
    : buffer(nullptr)
    , capacity(0)
    , length(0)
    , unitSize(1)
    , step(1.2) {
    }
    
    IBuffer::~IBuffer() {
        if (nullptr != buffer) {
            free(buffer);
            buffer = nullptr;
        }
        capacity = 0;
        length = 0;
        unitSize = 1;
    }

    void* IBuffer::increase(int unit) {
        if (nullptr == buffer) {
            capacity = 4;
            buffer = malloc(capacity * unitSize);
            if (nullptr == buffer) {
                LOGD("IBuffer", "malloc fail");
                return nullptr;
            }
            memset(buffer, 0, capacity * unitSize);
        }
        if (length + unit > capacity) {
            capacity = ceil(capacity * step);
            if (capacity < length + unit) {
                capacity = length + unit;
            }
            buffer = realloc(buffer, capacity * unitSize);
            if (nullptr == buffer) {
                LOGD("IBuffer", "malloc fail");
                return nullptr;
            }
        }
        
        void* cur = (char*)buffer + length * unitSize;
        length += unit;

        return cur;
    }

    void* IBuffer::getBufferPoint(int pos) {
        if (nullptr == buffer) {
            return buffer;
        }
        if (pos >= length) {
            return nullptr;
        }
        if (-1 == pos) {
            return (char*)buffer + length * unitSize;
        } else {
            return (char*)buffer + pos * unitSize;
        }
    }
}