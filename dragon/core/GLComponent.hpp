//
//  GLComponent.hpp
//  dragon
//
//  Created by hugo on 16/6/12.
//
//

#ifndef GLComponent_hpp
#define GLComponent_hpp

#include "glfw3.h"
#include "glm.hpp"

#include "Component.hpp"

namespace dragon {
    class GLComponent : public Component {
    public:
        GLComponent();
        ~GLComponent();

        void setSize(const glm::vec2& size);
        void setClearColor(const glm::vec4& clr);

        void initGLFW();
        
        void clearGL();
        void swap();
        void check();

        virtual void onEnter() override;

        void onGLFWDrop(GLFWwindow* w, int count, const char** path);
        void onGLFWKey(GLFWwindow* w, int key , int code , int action, int mod);
        void onGLFWMouseButton(GLFWwindow* w, int button, int action, int mod);
        void onGLFWMouseMove(GLFWwindow* w, double x, double y);

    protected:
        GLFWwindow* window;

        glm::vec2 screenSize;
        glm::vec4 clearColor;
    };
}

#endif /* GLComponent_hpp */
