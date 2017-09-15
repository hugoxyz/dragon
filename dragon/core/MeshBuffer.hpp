//
//  MeshBuffer.hpp
//  dragon
//
//  Created by hugo on 16/6/20.
//
//

#ifndef MeshBuffer_hpp
#define MeshBuffer_hpp

#include "glfw3.h"
#include "Object.hpp"

namespace dragon {
    class MeshBuffer : public Object {
    public:
        MeshBuffer();
        MeshBuffer(int size);
        ~MeshBuffer();

        void expand(int size);
        void* getBuffer();
        void bindBufferIf(GLenum target, GLenum usage);
        GLuint getLocation();
        int getSize();

    protected:
        void* buffer;
        int size;
        GLuint location;

    };
}

#endif /* MeshBuffer_hpp */
