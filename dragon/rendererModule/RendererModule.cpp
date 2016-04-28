//
//  RendererModule.cpp
//  dragon
//
//  Created by hugo on 16/4/17.
//
//

#include <stdlib.h>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "RendererModule.hpp"
#include "RendererMessage.hpp"
#include "../core/Manager.hpp"
#include "../core/FileUtils.hpp"
#include "Logger.hpp"
#include "../fbxModule/FBXMessage.hpp"

namespace dragon {
    
    class glCallbacks {
    public:
        static void drop_cb(GLFWwindow* w, int count, const char** path) {
            const char* file = nullptr;
            Node* rendererNode = Manager::getInstance()->getChild("__renderer_mod");
            if (nullptr == rendererNode) {
                LOGE("glCallbacks", "rendererModule is null");
                return;
            }
            Node* fbxNode = Manager::getInstance()->getChild("__fbx_mod");
            if (nullptr == fbxNode) {
                LOGE("glCallbacks", "can't find fbx module");
                return;
            }
            
            for (auto i = 0; i < count; i ++) {
                file = *(path + i);
                LOGD("glCallbacks", file);

                FBXMessage* msg = new FBXMessage();
                msg->receiver = fbxNode->getId();
                msg->sender = rendererNode->getId();
                msg->setFilePath(file);
                Manager::getInstance()->sendMsg(msg);
            }
        }
    };
    
    RendererModule* RendererModule::instance = nullptr;
    
    RendererModule::RendererModule()
    : window(nullptr) {
        name = "__renderer_mod";
        
        width = 960;
        height = 640;
        
        cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
        cameraFocus = glm::vec3(0.0f, 0.0f, 0.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        moduleMatrix = glm::mat4(1.0f);
        viewMatrix = glm::lookAt(cameraPos, cameraFocus, cameraUp);
        projectMatrix = glm::perspective(45.0f, (float)(width/height), 0.1f, 100.0f);
    }

    RendererModule::~RendererModule() {
        program->release();
    }
    
    bool RendererModule::init() {
        if (GL_TRUE != glfwInit()) {
            return false;
        }
        
        window = glfwCreateWindow(width, height, "dragon", nullptr, nullptr);
        if (nullptr == window) {
            glfwTerminate();
            return false;
        }
        
        glfwMakeContextCurrent(window);
        glfwSetDropCallback(window, glCallbacks::drop_cb);
        
        createGLProgram();
        glGenBuffers(1, &vertexesBuf);
        
        float attr[] = {
            -1, -1, 0,
            1,  -1, 0,
            0,  1, 0
        };
        
        setBufferData(vertexesBuf, attr, sizeof(attr));
        
        program->use();
        
        glm::mat4 mat = moduleMatrix * viewMatrix;
        program->setUnifrom("um4MVMatrix", glm::value_ptr(mat), 16);
        program->setUnifrom("um4PMatrix", glm::value_ptr(projectMatrix), 16);
        
        glClearColor(0.5, 0.5, 0.5, 0.5);
        return true;
    }
    
    bool RendererModule::windowShouldClose() {
        return glfwWindowShouldClose(window);
    }

    void RendererModule::update(int dt) {
        if (windowShouldClose()) {
            Manager::getInstance()->exit();
            return;
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        do {
            if (nullptr != program) {
                GLuint vertextLocation = 0;
                if (!program->getAttributeLocation("av3Vertex", &vertextLocation)) {
                    break;
                }
                glBindBuffer(GL_ARRAY_BUFFER, vertexesBuf);
                glEnableVertexAttribArray(vertextLocation);
                glVertexAttribPointer(vertextLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
                glDrawArrays(GL_LINE_LOOP, 0, 3);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
            
        } while (false);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void RendererModule::onStateEvent(StateEvent e) {
        switch (e) {
            case StateEvent::ENTER: {
                break;
            }
            case StateEvent::SUSPEND: {
                break;
            }
            case StateEvent::RESUME: {
                break;
            }
            case StateEvent::LEAVE: {
                glfwTerminate();
                break;
            }
            default:
                break;
        }
    }
    
    void RendererModule::onMessage(Message *msg) {
        RendererMessage *m = dynamic_cast<RendererMessage*>(msg);
        if (nullptr == m) {
            return;
        }
        
        
    }

    void RendererModule::createGLProgram() {
        char* vs = nullptr;
        char* fs = nullptr;
        int vsLen = 0;
        int fsLen = 0;
        bool ret = false;

        ret = FileUtils::getInstance()->read("./resources/shader/default.vsh", (unsigned char**)&vs, &vsLen, true);
        ret = ret && FileUtils::getInstance()->read("./resources/shader/default.fsh", (unsigned char**)&fs, &fsLen, true);
        if (!ret) {
            if (nullptr != vs) {
                free(vs);
            }
            if (nullptr != fs) {
                free(fs);
            }
            return;
        }

        program = new GLProgram();
        program->retain();
        program->compile(vs, fs);
        if (nullptr != vs) {
            free(vs);
        }
        if (nullptr != fs) {
            free(fs);
        }
    }

    void RendererModule::setBufferData(GLuint buffer, void* data, int size) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}