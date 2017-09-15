//
//  CameraComponent.cpp
//  dragon
//
//  Created by hugo on 16/5/12.
//
//

#include "glm/ext.hpp"
#include "CameraComponent.hpp"
#include "TransformComponent.hpp"
#include "EventComponent.hpp"
#include "Node.hpp"
#include "Manager.hpp"

namespace dragon {
    
    CameraComponent::CameraComponent()
    : width(960)
    , height(640)
    , near(0.1)
    , far(100)
    , angle(60)
    , projectMatrixDirty(true)
    , viewMatrixDirty(true)
    , projection(Perspective) {
        focus = glm::vec3(0, 0, -1);
        up = glm::vec3(0, 1, 0);
    }
    
    CameraComponent::~CameraComponent() {
    }
    
    void CameraComponent::moveFocus(const glm::vec3& v) {
        focus += v;
        viewMatrixDirty = true;
        
        Manager::getInstance()->postEvent(static_cast<int>(EventComponent::Event::EVENT_CAMERA_VIEW_CHANGE), nullptr);
    }
    
    const glm::mat4& CameraComponent::getProjectMatrix() {
        if (projectMatrixDirty) {
            switch (projection) {
                case Perspective: {
                    projectMatrix = glm::perspective(angle, width/height, near, far);
                    break;
                }
                case Orthogonal: {
                    projectMatrix = glm::ortho(0.0f, width, 0.0f, height, near, far);
                    break;
                }
                default: {
                    assert(false);
                    break;
                }
            }
            projectMatrixDirty = false;
        }
        return projectMatrix;
    }

    const glm::mat4& CameraComponent::getViewMatrix() {
        if (nullptr == host) {
            return viewMatrix;
        }
        TransformComponent *trans = host->getComponent<TransformComponent>();
        if (nullptr == trans) {
            return viewMatrix;
        }
        
        if (viewMatrixDirty) {
            viewMatrix = glm::lookAt(trans->getPosition(), focus, up);
            viewMatrixDirty = false;
        }
        
        return viewMatrix;
    }
    
    void CameraComponent::setFocus(const glm::vec3& v) {
        focus = v;
        viewMatrixDirty = true;

        Manager::getInstance()->postEvent(static_cast<int>(EventComponent::Event::EVENT_CAMERA_VIEW_CHANGE), nullptr);
    }
    
    void CameraComponent::setProjection(Projection p) {
        projection = p;
    }
    
    CameraComponent::Projection CameraComponent::getProjection() {
        return projection;
    }

    void CameraComponent::apply(GLProgram* program) {
        glm::mat4 project = getProjectMatrix();
        program->setUnifrom("um4PMatrix", glm::value_ptr(project), 16);
        
        glm::mat4 viewMat = getViewMatrix();
        program->setUnifrom("um4VMatrix", glm::value_ptr(viewMat), 16);
    }

}
