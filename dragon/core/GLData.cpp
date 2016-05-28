//
//  MemoryList.cpp
//  dragon
//
//  Created by hugo on 16/5/24.
//
//

#include <stdlib.h>

#include "GLData.hpp"
#include "Logger.hpp"

namespace dragon {
    GLData::GLData()
    : capacity(0)
    , length(0)
    , unitSize(1)
    , memory(nullptr)
    , location(0)
    , bufferType(0)
    , locationValid(false)
    , dirty(true)
    , vertexOffset(-1)
    , colorOffset(-1)
    , normalOffset(-1)
    , textureOffset(-1) {
        genGLBufferIf();
    }
    
    GLData::~GLData() {
        cleanBuffer();

        FREEIF(memory);
        capacity = 0;
        length = 0;
        unitSize = 0;
    }
    
    void GLData::setUnitSize(int s) {
        unitSize = s;
    }
    
    void* GLData::addMemory(void *p) {
        if (nullptr == memory) {
            capacity = 2;
            memory = malloc(capacity * unitSize);
            if (nullptr == memory) {
                LOGD("MemoryList", "malloc fail");
                return nullptr;
            }
            memset(memory, 0, capacity * unitSize);
        }
        if (length == capacity) {
            capacity = capacity * 2;
            memory = realloc(memory, capacity * unitSize);
            if (nullptr == memory) {
                LOGD("MemeoryList", "malloc fail");
                return nullptr;
            }
        }
        
        void *dest = (char*)memory + length * unitSize;
        if (nullptr != p) {
            memcpy(dest, p, unitSize);
        }
        length += 1;
        setDirtyFlag(true);
        
        return dest;
    }
    
    void* GLData::getMemory(int pos) {
        if (nullptr == memory) {
            return nullptr;
        }
        if (pos >= length) {
            return nullptr;
        }
        return (char*)memory + pos * unitSize;
    }
    
    int GLData::getMemoryLength() {
        return length;
    }
    
    int GLData::getUnitSize() {
        return unitSize;
    }
    
    void GLData::setGLBufferType(GLenum type) {
        bufferType = type;
    }
    
    void GLData::genGLBufferIf() {
        if (locationValid) {
            return;
        }
        glGenBuffers(1, &location);
        locationValid = true;
    }
    
    void GLData::cleanBuffer() {
        if (locationValid) {
            locationValid = false;
            glDeleteBuffers(1, &location);
        }
    }
    
    void GLData::bindData(GLenum usage) {
        genGLBufferIf();
        glBindBuffer(bufferType, location);
        glBufferData(bufferType, length * unitSize, memory, usage);
        setDirtyFlag(false);
    }
    
    GLuint GLData::getGLLocation() {
        return location;
    }
    
    void GLData::setDirtyFlag(bool dirty) {
        this->dirty = dirty;
    }
    
    bool GLData::isDirty() {
        return dirty;
    }

    void GLData::setVertexOffset(char offset) {
        vertexOffset = offset;
    }
    
    void GLData::setColorOffset(char offset) {
        colorOffset = offset;
    }
    
    void GLData::setNormalOffset(char offset) {
        normalOffset = offset;
    }
    
    void GLData::setTextureOffset(char offset) {
        textureOffset = offset;
    }
    
    char GLData::getVertexOffset() {
        return vertexOffset;
    }
    
    char GLData::getColorOffset() {
        return colorOffset;
    }
    
    char GLData::getNormalOffset() {
        return normalOffset;
    }
    
    char GLData::getTextureOffset() {
        return textureOffset;
    }
    
}