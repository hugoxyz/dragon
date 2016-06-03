//
//  TransformComponent.cpp
//  dragon
//
//  Created by hugo on 16/5/4.
//
//

#include "TransformComponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace dragon {
    
    TransformComponent::TransformComponent()
    : moduleMatrixDirty(true) {
        size = glm::vec3(1);
    }
    
    TransformComponent::~TransformComponent() {
    }
    
    void TransformComponent::setPosition(const glm::vec3& v) {
        position = v;
        moduleMatrixDirty = true;
    }
    
    void TransformComponent::setScale(const glm::vec3& v) {
        size = v;
        moduleMatrixDirty = true;
    }
    
    void TransformComponent::setRotation(const glm::vec3& v) {
        rotation = v;
        moduleMatrixDirty = true;
    }
    
    void TransformComponent::move(const glm::vec3& v) {
        position += v;
        moduleMatrixDirty = true;
    }
    
    void TransformComponent::scale(const glm::vec3& v) {
        size += v;
        moduleMatrixDirty = true;
    }
    
    void TransformComponent::rotate(const glm::vec3& v) {
        rotation += v;
        moduleMatrixDirty = true;
    }
    
    const glm::vec3& TransformComponent::getPosition() {
        return position;
    }

    const glm::vec3& TransformComponent::getScale() {
        return size;
    }
    
    const glm::vec3& TransformComponent::getRotation() {
        return rotation;
    }
    
    const glm::mat4& TransformComponent::getModuleMatrix() {
        if (moduleMatrixDirty) {
            moduleMatrix = glm::scale(glm::mat4(1.0f), size);
            glm::quat q = glm::quat(rotation);
            moduleMatrix *= glm::toMat4(q);
            moduleMatrix = glm::translate(moduleMatrix, position);
            
            moduleMatrixDirty = false;
        }
        
        return moduleMatrix;
    }
    
    bool TransformComponent::isDirty() {
        return moduleMatrixDirty;
    }
}