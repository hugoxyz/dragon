//
//  RectComponent.cpp
//  dragon
//
//  Created by hugo on 16/6/14.
//
//

//#include "Node.hpp"
#include "Manager.hpp"
#include "RectComponent.hpp"
#include "TransformComponent.hpp"
#include "GLProgramCache.hpp"

namespace dragon {

    RectComponent::RectComponent()
    : borderWidth(1)
    , borderColor(glm::vec4(1, 1, 1, 0))
    , fillColor(glm::vec4(1, 1, 1, 0))
    , glProgram(nullptr) {
        glProgram = GLProgramCache::getInstance()->getProgram("./resources/shader/default.vsh", "./resources/shader/default.fsh");
        glProgram->retain();
    }
    
    RectComponent::~RectComponent() {
        if  (glProgram) {
            glProgram->release();
            glProgram = nullptr;
        }
    }
    
    void RectComponent::onUpdate() {
        if (nullptr == host) {
            return;
        }
        TransformComponent* trans = host->getTransComponent();
        assert(nullptr != trans);
        glm::mat4 mat;
        trans->toGlobalCoorMatrix(mat);
        
        if (nullptr == camera) {
            camera = Manager::getInstance()->findNode("__camera_node");
        }
        glProgram->use();
        host->apply(glProgram);
    }
    
    void RectComponent::setFillColor(const glm::vec4& color) {
        fillColor = color;
    }
    
    void RectComponent::setBorderColor(const glm::vec4& color) {
        borderColor = color;
    }
    
    void RectComponent::setBorderWidth(int w) {
        borderWidth = w;
    }

}