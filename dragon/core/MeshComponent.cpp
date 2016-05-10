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
#include "../rendererModule/GLProgramCache.hpp"
#include "glm.hpp"

namespace dragon {
    
    MeshComponent::MeshComponent()
    : vertexes(nullptr)
    , vertexCapacity(0)
    , vertexLength(0)
    , glBuffer(0)
    , glBufferInvalid(true) {
        glGenBuffers(1, &glBuffer);
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
        glBufferInvalid = true;
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
    
    void MeshComponent::onPreUpdate() {
        if (glBufferInvalid) {
            glBufferInvalid = false;
            glBindBuffer(GL_ARRAY_BUFFER, glBuffer);
            glBufferData(GL_ARRAY_BUFFER, vertexLength*sizeof(gl::Vertex), vertexes, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
    
    void MeshComponent::onUpdate() {
        GLProgram* program = GLProgramCache::getInstance()->getProgram("./resources/shader/default.vsh", "./resources/shader/default.fsh");
//        program->setUnifrom("um4MVMatrix", glm::value_ptr(mat), 16);
//        program->setUnifrom("um4PMatrix", glm::value_ptr(projectMatrix), 16);
        
        glBindBuffer(GL_ARRAY_BUFFER, glBuffer);
        glDrawArrays(GL_LINES, 0, vertexLength*3);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void MeshComponent::onAfterUpdate() {
    }
    
}