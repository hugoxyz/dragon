//
//  RendererModule.cpp
//  dragon
//
//  Created by hugo on 16/4/17.
//
//

#include <stdlib.h>
#include <functional>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "RendererModule.hpp"
#include "EventRenderer.hpp"
#include "../core/Manager.hpp"
#include "../core/FileUtils.hpp"
#include "Logger.hpp"
#include "../fbxModule/FBXMessage.hpp"
#include "../core/EventInput.hpp"
#include "../core/EventComponent.hpp"
#include "../core/CameraComponent.hpp"

namespace dragon {
    
    class glCallbacks {
    public:
        static void drop_cb(GLFWwindow* w, int count, const char** path) {
            const char* file = nullptr;
            EventInput* event = new EventInput(EventInput::EventInputType::DROP_FILE);
            std::vector<std::string> paths;

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
                paths.push_back(file);
            }
            event->setPaths(paths);
            Manager::getInstance()->postEvent(static_cast<int>(EventComponent::Event::EVENT_INPUT), event);
        }
    };
    
    RendererModule* RendererModule::instance = nullptr;
    
    RendererModule::RendererModule()
    : window(nullptr)
    , cameraid(0) {
        name = "__renderer_mod";
        
        width = 960;
        height = 640;
        
        cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
        cameraFocus = glm::vec3(0.0f, 0.0f, 0.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        moduleMatrix = glm::mat4(1.0f);
        viewMatrix = glm::lookAt(cameraPos, cameraFocus, cameraUp);
        projectMatrix = glm::perspective(45.0f, (float)(width/height), 0.1f, 100.0f);
        
        step();
    }

    RendererModule::~RendererModule() {
        program->release();
        glfwTerminate();
    }
    
    RendererModule* RendererModule::getInstance() {
        if (nullptr == instance) {
            instance = new RendererModule();
        }
        
        return instance;
    }
    
    void RendererModule::createCameraNode() {
        auto camera = new Node();
        camera->createTransformIf();
        camera->addComponent(new CameraComponent());
        addChild(camera);
        cameraid = camera->getId();
    }
    
    Node* RendererModule::getCameraNode() {
        return getChild(cameraid);
    }
    
    void RendererModule::step() {
        if (GL_TRUE != glfwInit()) {
            return ;
        }
        
        window = glfwCreateWindow(width, height, "dragon", nullptr, nullptr);
        if (nullptr == window) {
            glfwTerminate();
            return ;
        }
        
        glfwMakeContextCurrent(window);
        glfwSetDropCallback(window, glCallbacks::drop_cb);
        
        createGLProgram();
        glGenBuffers(1, &vertexesBuf);

        EventRenderer::Vertex vertex[3] = {};
        vertex[0].x = -1;
        vertex[0].y = -1;
        vertex[0].z = 0;
        vertex[1].x = 1;
        vertex[1].y = -1;
        vertex[1].z = 0;
        vertex[2].x = 0;
        vertex[2].y = 1;
        vertex[2].z = 0;

        setBufferData(vertexesBuf, vertex, sizeof(vertex));
        
        program->use();
        
        glm::mat4 mat = moduleMatrix * viewMatrix;
        program->setUnifrom("um4MVMatrix", glm::value_ptr(mat), 16);
        program->setUnifrom("um4PMatrix", glm::value_ptr(projectMatrix), 16);
        
        glClearColor(0.5, 0.5, 0.5, 0.5);
        
        EventComponent* event = Manager::getInstance()->getComponent<EventComponent>();
        if (nullptr != event) {
            event->addObserver(static_cast<int>(EventComponent::Event::EVENT_RENDERER),
                               std::bind(&RendererModule::onRendererEvent,
                                         this,
                                         std::placeholders::_1,
                                         std::placeholders::_2,
                                         std::placeholders::_3));
            event->addObserver(static_cast<int>(EventComponent::Event::EVENT_RENDERER_NODE),
                               std::bind(&RendererModule::onRendererNode,
                                         this,
                                         std::placeholders::_1,
                                         std::placeholders::_2,
                                         std::placeholders::_3));
        }
        return ;
    }
    
    bool RendererModule::windowShouldClose() {
        return glfwWindowShouldClose(window);
    }

    void RendererModule::preUpdate() {}
    
    void RendererModule::afterUpdate() {}
    
    void RendererModule::update() {
        if (windowShouldClose()) {
            Manager::getInstance()->exit();
            return;
        }

        Module::preUpdate();
        Module::update();
        Module::afterUpdate();
    }
    
    void RendererModule::onPreUpdate() {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    void RendererModule::onAfterUpdate() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    void RendererModule::onUpdate() {}

    void RendererModule::onMessage(Message *msg) {
        EventRenderer *m = dynamic_cast<EventRenderer*>(msg);
        if (nullptr == m) {
            return;
        }
    }
    
    void RendererModule::onRendererEvent(int event, dragon::Object *data, dragon::Object *userData) {
        if (event == static_cast<int>(EventComponent::Event::EVENT_RENDERER)) {
            EventRenderer* event = dynamic_cast<EventRenderer*>(data);
            if (nullptr == event) {
                return;
            }
            EventRenderer::Vertex *pVertex = nullptr;
            int size = 0;
            event->getVertex(&pVertex, &size);
            setBufferData(vertexesBuf, pVertex, size * sizeof(EventRenderer::Vertex));
        } else {
            LOGD("RendererModule", "unknow event:%d", event);
        }
    }
    
    void RendererModule::onRendererNode(int event, dragon::Object *data, dragon::Object *userData) {
        if (event == static_cast<int>(EventComponent::Event::EVENT_RENDERER_NODE)) {
            Node* node = dynamic_cast<Node*>(data);
            if (nullptr == node) {
                return;
            }
            
            //removeAllChild();
            addChild(node);
        } else {
            LOGD("RendererModule", "unknow event:%d", event);
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