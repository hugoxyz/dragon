//
//  RendererModule.hpp
//  dragon
//
//  Created by hugo on 16/4/17.
//
//

#ifndef RendererModule_hpp
#define RendererModule_hpp

#include "glm/glm.hpp"

#include "../core/Module.hpp"
#include "GLProgram.hpp"
#include "glfw/glfw3.h"

namespace dragon {

    class RendererModule : public Module {
    public:

        RendererModule();
        ~RendererModule();
        
        static RendererModule* self();
        
        virtual void onInit();
        virtual void update(int dt);
        bool windowShouldClose();
        void onStateEvent(StateEvent e);
        virtual void onMessage(Message* msg);
        
    protected:
        void createGLProgram();
        void setBufferData(GLuint buffer, void* data, int size);
        
        void onRendererEvent(int event, Object* data, Object* userData);
        void onRendererNode(int event, Object *data, Object *userData);

    protected:
        int width;
        int height;
        GLFWwindow* window;
        GLProgram* program;

        GLuint vertexesBuf;
        
        glm::mat4 moduleMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectMatrix;
        
        glm::vec3 cameraPos;
        glm::vec3 cameraFocus;
        glm::vec3 cameraUp;

    private:
        static RendererModule* instance;
    };
}

#endif /* RendererModule_hpp */
