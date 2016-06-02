//
//  TransformComponent.hpp
//  dragon
//
//  Created by hugo on 16/5/4.
//
//

#ifndef TransformComponent_hpp
#define TransformComponent_hpp

#include "Component.hpp"
#include "glm.hpp"

namespace dragon {
    class TransformComponent : public Component {
    public:
        
        TransformComponent();
        ~TransformComponent();

        void setPosition(const glm::vec3& v);
        void setScale(const glm::vec3& v);
        void setRotation(const glm::vec3& v);
        
        void move(const glm::vec3& v);
        void scale(const glm::vec3& v);
        void rotate(const glm::vec3& v);

        const glm::vec3& getPosition();
        const glm::vec3& getScale();
        const glm::vec3& getRotation();
        
        const glm::mat4& getModuleMatrix();
        
        bool isDirty();

    protected:
        glm::vec3 position;
        glm::vec3 size;
        glm::vec3 rotation;
        
        glm::mat4 moduleMatrix;
        bool moduleMatrixDirty;
    };
}

#endif /* TransformComponent_hpp */
