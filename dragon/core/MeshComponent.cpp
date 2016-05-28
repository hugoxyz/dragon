//
//  MeshComponent.cpp
//  dragon
//
//  Created by hugo on 16/5/6.
//
//

#include <stdlib.h>

#include "gl-mac.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "MeshComponent.hpp"
#include "Logger.hpp"
#include "../rendererModule/GLProgramCache.hpp"
#include "Manager.hpp"
#include "EventComponent.hpp"
#include "CameraComponent.hpp"
#include "TransformComponent.hpp"

namespace dragon {
    
    MeshComponent::MeshComponent()
    : vertexes(nullptr)
    , glBuffer(0)
    , glBufferInvalid(true)
    , cameraObserveTag(0)
    , cameraViewObserveTag(0)
    , projectMatrixDirty(true)
    , viewMatrixDirty(true)
    , moduleMatrixDirty(true)
    , program(nullptr) {
        vertexIndexVec.clear();
        glGenBuffers(1, &glBuffer);
    }
    
    MeshComponent::~MeshComponent() {
        if (nullptr != program) {
            program->release();
        }
        for(auto idx : vertexIndexVec) {
            idx->release();
        }
        vertexIndexVec.clear();
        if (nullptr != vertexes) {
            vertexes->release();
            vertexes = nullptr;
        }
    }
    
//    void MeshComponent::createVertexesIf(int length) {
//        if (vertexCapacity >= length) {
//            vertexLength = 0;
//            return ;
//        }
//        FREEIF(vertexes);
//        vertexes = (gl::Vertex*)malloc(sizeof(gl::Vertex)*length);
//        if (nullptr == vertexes) {
//            LOGD("MeshComponent", "vertexes is null");
//            return;
//        }
//        vertexCapacity = length;
//        memset(vertexes, 0, sizeof(gl::Vertex)*length);
//    }
//    
//    void MeshComponent::addVertex(const gl::Vertex& vertex) {
//        glBufferInvalid = true;
//        if (vertexLength == vertexCapacity) {
//            LOGD("MeshComponent", "vertex capacity not enough");
//            return;
//        }
//        gl::Vertex* cur = vertexes + vertexLength;
//        memcpy(cur, &vertex, sizeof(gl::Vertex));
//        vertexLength += 1;
//    }
//    
//    gl::Vertex* MeshComponent::getVertex() {
//        return vertexes;
//    }
    
    void MeshComponent::addVertexes(GLData* v) {
        vertexes = v;
        vertexes->retain();
    }
    
    void MeshComponent::addVertexIndex(GLData* idx) {
        idx->retain();
        vertexIndexVec.push_back(idx);
    }
    
    void MeshComponent::setShaderPath(const std::string& vshader, const std::string& fshader) {
        vshader_path = vshader;
        fshader_path = fshader;
    }
    
    void MeshComponent::onResume() {
        EventComponent* event = Manager::getInstance()->getComponent<EventComponent>();
        if (nullptr != event) {
            cameraObserveTag = event->addObserver(static_cast<int>(EventComponent::Event::EVENT_CAMERA_PROJECTION_CHANGE),
                                                  std::bind(&MeshComponent::onCameraProject,
                                                            this,
                                                            std::placeholders::_1,
                                                            std::placeholders::_2,
                                                            std::placeholders::_3));
            cameraViewObserveTag = event->addObserver(static_cast<int>(EventComponent::Event::EVENT_CAMERA_VIEW_CHANGE),
                                                      std::bind(&MeshComponent::onCameraView,
                                                                this,
                                                                std::placeholders::_1,
                                                                std::placeholders::_2,
                                                                std::placeholders::_3));
        }
        if (nullptr == program) {
            program = GLProgramCache::getInstance()->getProgram(vshader_path, fshader_path);
            if (nullptr != program) {
                program->retain();
                projectMatrixDirty = true;
            }
        }
    }
    
    void MeshComponent::onSuspend() {
        EventComponent* event = Manager::getInstance()->getComponent<EventComponent>();
        if (nullptr != event) {
            event->removeObserver(static_cast<int>(EventComponent::Event::EVENT_CAMERA_PROJECTION_CHANGE), cameraObserveTag);
            event->removeObserver(static_cast<int>(EventComponent::Event::EVENT_CAMERA_VIEW_CHANGE), cameraViewObserveTag);
        }
    }
    
    void MeshComponent::onPreUpdate() {
        if (nullptr == program) {
            return;
        }
        
        if (vertexes->isDirty()) {
            vertexes->setDirtyFlag(false);
            vertexes->setGLBufferType(GL_ARRAY_BUFFER);
            vertexes->bindData();
            GLuint location;
            if (vertexes->getVertexOffset() >= 0) {
                if (program->getAttributeLocation("av3Vertex", &location)) {
                    glEnableVertexAttribArray(location);
                    glVertexAttribPointer(location, 3, GL_FLOAT, false, vertexes->getUnitSize(), (void*)vertexes->getVertexOffset());
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                }
            }
            if (vertexes->getNormalOffset() >= 0) {
                if (program->getAttributeLocation("av3Normal", &location)) {
                    glEnableVertexAttribArray(location);
                    glVertexAttribPointer(location, 3, GL_FLOAT, false, vertexes->getUnitSize(), (void*)vertexes->getNormalOffset());
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                }
            }
            if (vertexes->getTextureOffset() >= 0) {
                if (program->getAttributeLocation("av2Texture", &location)) {
                    glEnableVertexAttribArray(location);
                    glVertexAttribPointer(location, 3, GL_FLOAT, false, vertexes->getUnitSize(), (void*)vertexes->getTextureOffset());
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                }
            }
        }
        
        for (auto vertexIndex : vertexIndexVec) {
            if (vertexIndex->isDirty()) {
                vertexIndex->setDirtyFlag(false);
                vertexIndex->setGLBufferType(GL_ELEMENT_ARRAY_BUFFER);
                vertexIndex->bindData();
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }
        }

        if (viewMatrixDirty || projectMatrixDirty) {
            Node* camera = Manager::getInstance()->getChild("__camera_node");
            CameraComponent* comp = nullptr == camera? nullptr : camera->getComponent<CameraComponent>();
            if (nullptr != comp) {
                program->use();
                if (projectMatrixDirty) {
                    glm::mat4 project = comp->getProjectMatrix();
                    program->setUnifrom("um4PMatrix", glm::value_ptr(project), 16);
                    projectMatrixDirty = false;
                }
                if (viewMatrixDirty) {
                    glm::mat4 viewMat = comp->getViewMatrix();
                    program->setUnifrom("um4VMatrix", glm::value_ptr(viewMat), 16);
                    viewMatrixDirty = false;
                }
            }
        }
        if (moduleMatrixDirty) {
            TransformComponent *trans = nullptr;
            if ( nullptr != host) {
                trans = host->getComponent<TransformComponent>();
            }
            if (nullptr != trans) {
                program->use();
                glm::mat4 m = trans->getModuleMatrix();
                program->setUnifrom("um4MMatrix", glm::value_ptr(m), 16);
                moduleMatrixDirty = false;
            }
        }
    }
    
    void MeshComponent::onUpdate() {
        if (nullptr == program) {
            return;
        }
        program->use();

        glBindBuffer(GL_ARRAY_BUFFER, vertexes->getGLLocation());
        for (auto vertexIndex : vertexIndexVec) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndex->getGLLocation());
//            glDrawArrays(GL_LINES, 0, vertexIndex->getMemoryLength());
            glDrawElements(GL_LINES, vertexIndex->getMemoryLength(), GL_UNSIGNED_INT, nullptr);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    void MeshComponent::onAfterUpdate() {
    }
    
    void MeshComponent::onCameraProject(int event, Object* data, Object* userData) {
        projectMatrixDirty = true;
    }
    
    void MeshComponent::onCameraView(int event, Object* data, Object* userData) {
        viewMatrixDirty = true;
    }
    
}