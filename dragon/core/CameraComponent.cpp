//
//  CameraComponent.cpp
//  dragon
//
//  Created by hugo on 16/5/12.
//
//

#include "glm/ext.hpp"
#include "CameraComponent.hpp"

namespace dragon {
    
    CameraComponent::CameraComponent()
    : width(960)
    , height(640)
    , near(0.1)
    , far(100)
    , angle(45)
    , projectMatrixDirty(true)
    , viewMatrixDirty(true) {
        position = glm::vec3(0, 0, 10);
        focus = glm::vec3(0, 0, 0);
        up = glm::vec3(0, 1, 0);
    }
    
    CameraComponent::~CameraComponent() {
    }
    
    const glm::mat4& CameraComponent::getProjectMatrix() {
        if (projectMatrixDirty) {
            projectMatrix = glm::perspective(angle, width/height, near, far);
            projectMatrixDirty = false;
        }
        return projectMatrix;
    }

    const glm::mat4& CameraComponent::getViewMatrix() {
        if (viewMatrixDirty) {
            viewMatrix = glm::lookAt(position, focus, up);
            viewMatrixDirty = false;
        }
        
        return viewMatrix;
    }

}
