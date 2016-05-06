//
//  MeshComponent.hpp
//  dragon
//
//  Created by hugo on 16/5/6.
//
//

#ifndef MeshComponent_hpp
#define MeshComponent_hpp

#include "Component.hpp"
#include "GLStructDefine.h"

namespace dragon {
    class MeshComponent : public Component {
    public:
        MeshComponent();
        ~MeshComponent();

        void createVertexesIf(int length);
        void addVertex(const gl::Vertex& vertex);
        gl::Vertex* getVertex();

    protected:
        gl::Vertex* vertexes;
        int vertexLength;
        int vertexCapacity;
    };
}

#endif /* MeshComponent_hpp */
