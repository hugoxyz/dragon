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

#include "GLProgram.hpp"


namespace dragon {
    class CameraComponent : public Component {
    public:
        
        enum Projection {
            Orthogonal,
            Perspective
        };
        
        CameraComponent();
        ~CameraComponent();
        
        void moveFocus(const glm::vec3& v);
        const glm::mat4& getProjectMatrix();
        const glm::mat4& getViewMatrix();
        
        void setFocus(const glm::vec3& v);
        void setProjection(Projection p);

        void apply(GLProgram* program) override;

    protected:
        float width;
        float height;
        float near;
        float far;
        float angle;
        glm::mat4 projectMatrix;
        bool projectMatrixDirty;

        glm::vec3 focus;
        glm::vec3 up;
        glm::mat4 viewMatrix;
        bool viewMatrixDirty;

        Projection projection;
    };
}

#endif /* CameraComponent_hpp */
