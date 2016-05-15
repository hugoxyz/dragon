//
//  CameraComponent.hpp
//  dragon
//
//  Created by hugo on 16/5/12.
//
//

#ifndef CameraComponent_hpp
#define CameraComponent_hpp

#include "glm/glm.hpp"
#include "Component.hpp"


namespace dragon {
    class CameraComponent : public Component {
    public:
        
        CameraComponent();
        ~CameraComponent();
        
        const glm::mat4& getProjectMatrix();
        const glm::mat4& getViewMatrix();
        
    protected:
        float width;
        float height;
        float near;
        float far;
        float angle;
        glm::mat4 projectMatrix;
        bool projectMatrixDirty;
        
        glm::vec3 position;
        glm::vec3 focus;
        glm::vec3 up;
        glm::mat4 viewMatrix;
        bool viewMatrixDirty;
    };
}

#endif /* CameraComponent_hpp */
