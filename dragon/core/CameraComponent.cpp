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
    , angle(45)
    , projectMatrixDirty(true)
    , viewMatrixDirty(true) {
        focus = glm::vec3(0, 0, 0);
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
            projectMatrix = glm::perspective(angle, width/height, near, far);
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

}
