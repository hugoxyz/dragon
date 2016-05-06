//
//  GLStructDefine.h
//  dragon
//
//  Created by hugo on 16/5/6.
//
//

#ifndef GLStructDefine_h
#define GLStructDefine_h

namespace dragon {
    namespace gl {
        typedef struct {
            float x;
            float y;
            float z;
        } Vertex;
        
        typedef struct {
            float r;
            float g;
            float b;
            float a;
        } Color;
        
        typedef struct {
            float x;
            float y;
            float z;
        } Normal;
        
        typedef struct {
            float u;
            float v;
        } UV;
    }
}


#endif /* GLStructDefine_h */
