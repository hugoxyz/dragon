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

        void setPosition(const glm::vec3& v);
        void setScale(const glm::vec3& v);
        void setRotation(const glm::vec3& v);

        const glm::vec3& getPosition();
        const glm::vec3& getScale();
        const glm::vec3& getRotation();

    protected:
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
    };
}

#endif /* TransformComponent_hpp */
