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
#include "glfw3.h"

namespace dragon {

    class RendererModule : public Module {
    public:

        RendererModule();
        ~RendererModule();

        static RendererModule* getInstance();

        void createCameraNode();
        Node* getCameraNode();

        void step();
        bool windowShouldClose();

        virtual void onMessage(Message* msg);
        
        /*
         * Life Cycle
         */
        virtual void preUpdate() override;
        virtual void afterUpdate() override;
        virtual void update() override;
        virtual void onPreUpdate() override;
        virtual void onAfterUpdate() override;
        virtual void onUpdate() override;
        
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
        
        int cameraid;

    private:
        static RendererModule* instance;
    };
}

#endif /* RendererModule_hpp */
