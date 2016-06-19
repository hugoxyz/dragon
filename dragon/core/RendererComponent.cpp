//
//  RendererComponent.cpp
//  dragon
//
//  Created by hugo on 16/6/17.
//
//

#include "RendererComponent.hpp"

namespace dragon {
    RendererComponent::RendererComponent() {
    }
    
    RendererComponent::~RendererComponent() {
    }
    
    void RendererComponent::onUpdate() {
        if (nullptr == host) {
            return;
        }
    }
}