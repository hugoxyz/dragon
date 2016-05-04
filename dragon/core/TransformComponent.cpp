//
//  TransformComponent.cpp
//  dragon
//
//  Created by hugo on 16/5/4.
//
//

#include "TransformComponent.hpp"

namespace dragon {
    void TransformComponent::setPosition(const glm::vec3& v) {
        position = v;
    }
    
    void TransformComponent::setScale(const glm::vec3& v) {
        scale = v;
    }
    
    void TransformComponent::setRotation(const glm::vec3& v) {
        rotation = v;
    }
}