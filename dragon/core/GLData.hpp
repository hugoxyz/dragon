//
//  MemoryList.hpp
//  dragon
//
//  Created by hugo on 16/5/24.
//
//

#ifndef GLBuffer_hpp
#define GLBuffer_hpp

#include "glfw3.h"
#include "Object.hpp"

namespace dragon {
    class GLData : public Object {
    public:

        GLData();
        ~GLData();

        void setUnitSize(int s);
        void* addMemory(void *p);
        void* getMemory(int pos = 0);

        int getMemoryLength();
        int getUnitSize();
        
        void setGLBufferType(GLenum type);
        void bindData(GLenum usage = GL_STATIC_DRAW);

        GLuint getGLLocation();
        
        void setDirtyFlag(bool dirty);
        bool isDirty();

        void genGLBufferIf();
        
        void setVertexOffset(char offset);
        void setColorOffset(char offset);
        void setNormalOffset(char offset);
        void setTextureOffset(char offset);
        char getVertexOffset();
        char getColorOffset();
        char getNormalOffset();
        char getTextureOffset();

    protected:
        void cleanBuffer();

    protected:
        void* memory;
        int unitSize;
        int capacity;
        int length;
        
        char vertexOffset;
        char colorOffset;
        char normalOffset;
        char textureOffset;

        bool dirty;
        bool locationValid;
        GLuint location;
        GLenum bufferType;
    };
}

#endif /* MemoryList_hpp */
