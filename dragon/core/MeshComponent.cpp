//
//  MeshComponent.cpp
//  dragon
//
//  Created by hugo on 16/5/6.
//
//

#include <stdlib.h>
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
    , vertexCapacity(0)
    , vertexLength(0)
    , glBuffer(0)
    , glBufferInvalid(true)
    , cameraObserveTag(0)
    , projectMatrixDirty(true)
    , viewMatrixDirty(true)
    , moduleMatrixDirty(true)
    , program(nullptr) {
        glGenBuffers(1, &glBuffer);
    }
    
    MeshComponent::~MeshComponent() {
        if (nullptr != program) {
            program->release();
        }
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
        }
    }
    
    void MeshComponent::onPreUpdate() {
        if (nullptr == program) {
            return;
        }
        if (glBufferInvalid) {
            glBufferInvalid = false;
            glBindBuffer(GL_ARRAY_BUFFER, glBuffer);
            glBufferData(GL_ARRAY_BUFFER, vertexLength*sizeof(gl::Vertex), vertexes, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
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
                    program->setUnifrom("um4MVMatrix", glm::value_ptr(viewMat), 16);
                    viewMatrixDirty = false;
                }
            }
        }
        if (moduleMatrixDirty) {
            TransformComponent *trans = getComponent<TransformComponent>();
            if (nullptr == trans) {
                program->use();
                glm::mat4 m = trans->getModuleMatrix();
                program->setUnifrom("", glm::value_ptr(m), 16);
                moduleMatrixDirty = false;
            }
        }
    }
    
    void MeshComponent::onUpdate() {
        if (nullptr == program) {
            return;
        }
        program->use();

        glBindBuffer(GL_ARRAY_BUFFER, glBuffer);
        glDrawArrays(GL_LINES, 0, vertexLength*3);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void MeshComponent::onAfterUpdate() {
    }
    
    void MeshComponent::onCameraProject(int event, Object* data, Object* userData) {
        projectMatrixDirty = true;
    }
    
}