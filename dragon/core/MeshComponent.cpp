//
//  MeshComponent.cpp
//  dragon
//
//  Created by hugo on 16/5/6.
//
//

#include <stdlib.h>
#include "MeshComponent.hpp"
#include "Logger.hpp"

namespace dragon {
    
    MeshComponent::MeshComponent()
    : vertexes(nullptr)
    , vertexCapacity(0)
    , vertexLength(0) {
    }
    
    MeshComponent::~MeshComponent() {
        FREEIF(vertexes);
    }
    
    void MeshComponent::createVertexesIf(int length) {
        if (vertexCapacity >= length) {
            vertexLength = 0;
            return ;
        }
        FREEIF(vertexes);
        vertexes = (gl::Vertex*)malloc(sizeof(gl::Vertex)*length);
        if (nullptr == vertexes) {
            LOGD("MeshComponent", "vertexes is null");
            return;
        }
        vertexCapacity = length;
        memset(vertexes, 0, sizeof(gl::Vertex)*length);
    }
    
    void MeshComponent::addVertex(const gl::Vertex& vertex) {
        if (vertexLength == vertexCapacity) {
            LOGD("MeshComponent", "vertex capacity not enough");
            return;
        }
        gl::Vertex* cur = vertexes + vertexLength;
        memcpy(cur, &vertex, sizeof(gl::Vertex));
        vertexLength += 1;
    }
    
    gl::Vertex* MeshComponent::getVertex() {
        return vertexes;
    }
    
}