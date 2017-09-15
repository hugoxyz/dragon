//
//  MeshBuffer.cpp
//  dragon
//
//  Created by hugo on 16/6/20.
//
//

#include <stdlib.h>

#include "Logger.hpp"
#include "MeshBuffer.hpp"

namespace dragon {
    MeshBuffer::MeshBuffer(int sizeOfByte)
    : buffer(nullptr)
    , size(sizeOfByte)
    , location(0) {
        buffer = malloc(size);
        if (nullptr == buffer) {
            LOGD("MeshBuffer", "malloc buffer fail");
        }
        glGenBuffers(1, &location);
    }
    
    MeshBuffer::MeshBuffer()
    : buffer(nullptr)
    , size(0)
    , location(0) {
        glGenBuffers(1, &location);
    }
    
    MeshBuffer::~MeshBuffer() {
        if (nullptr != buffer) {
            free(buffer);
            buffer = nullptr;
        }
        glDeleteBuffers(1, &location);
    }
    
    void MeshBuffer::expand(int sizeOfByte) {
        if (sizeOfByte == size) {
            return;
        }
        void* bufferN = nullptr;
        bufferN = malloc(sizeOfByte);
        if (nullptr == bufferN) {
            LOGD("MeshBuffer", "malloc buffer fail");
        }
        memset(bufferN, 0, sizeOfByte);

        if (nullptr != buffer) {
            memcpy(bufferN, buffer, std::min(size, sizeOfByte));
            free(buffer);
            buffer = nullptr;
            size = 0;
        }
        size = sizeOfByte;
    }
    
    void* MeshBuffer::getBuffer() {
        return buffer;
    }

    void MeshBuffer::bindBufferIf(GLenum target, GLenum usage) {
        glBindBuffer(target, location);
        glBufferData(target, size, buffer, usage);
        //glBindBuffer(target, 0);
    }
    
    GLuint MeshBuffer::getLocation() {
        return location;
    }
    
    int MeshBuffer::getSize() {
        return size;
    }
}

