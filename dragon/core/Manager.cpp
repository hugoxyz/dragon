//
//  Manager.cpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#include <unistd.h>
#include <thread>
#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "Manager.hpp"
#include "Logger.hpp"
#include "AutoReleasePoolMgr.hpp"
#include "EventComponent.hpp"
#include "CameraComponent.hpp"
#include "ControlComponent.hpp"
#include "TransformComponent.hpp"
#include "GLComponent.hpp"
#include "FileUtils.hpp"

#include "../uiModule/UIModule.hpp"
#include "../fbxModule/FBXModule.hpp"
#include "../rendererModule/RendererModule.hpp"

#include "SceneImporter.hpp"


namespace dragon {
    Manager* Manager::instance = nullptr;
    
    Manager* Manager::getInstance() {
        if (nullptr == instance) {
            instance = new Manager();
            instance->retain();
        }
        return instance;
    }
    
    Manager::Manager()
    : eventComponent(nullptr)
    , running(false)
    , fps(60)
    , mainNode(nullptr)
    , appName("DRAGON")
    , screenWidth(960)
    , screenHeight(640)
    , bgColorR(128)
    , bgColorG(128)
    , bgColorB(128)
    , bgColorA(128){
        FileUtils::getInstance()->addSearchPath("resources");
    }
    
    Manager::~Manager() {
        mainNode->release();
        eventComponent->release();
        glComponent->release();
    }

    void Manager::createMainNode() {
        assert(nullptr == mainNode);

        Node* n = new Node();
        addChild(n, false);
        mainNode = n;
        mainNode->retain();
    }
    
    void Manager::addChild(Node* n, bool bMain) {
        if (bMain) {
            mainNode->addChild(n);
        } else {
            Module::addChild(n);
            n->enter();
        }
    }

    void Manager::addComponent(Component* comp, const std::string& name) {
        Module::addComponent(comp, name);
        comp->enter();
    }
    
    void Manager::step() {
        msgs.clear();
        
        createMainNode();
        loadDefaultComponent();
        loadDefaultModule();
        
        config();
    }
    
    void Manager::config() {
        glComponent->setSize(glm::vec2(screenWidth, screenHeight));
        glComponent->setClearColor(glm::vec4(bgColorR/255.0, bgColorG/255.0, bgColorB/255.0, bgColorA/255.0));
        setName(appName);
    }
    
    void Manager::onDeinit() {
        if (nullptr != uiModule) {
            uiModule->release();
            uiModule = nullptr;
        }
    }
    
    void Manager::loadDefaultModule() {
//        Module* m = new UIModule();
//        addChild(m);
//        uiModule = m;
//        uiModule->retain();
//        
//        m = new FBXModule();
//        addChild(m);
//        
//        m = new RendererModule();
//        addChild(m);
        
        Node *n = new Node();
        n->createTransformIf();
        n->addComponent(new CameraComponent());
        n->addComponent(new ControlComponent());
        n->setName("__camera_node");
        addChild(n);
        
        TransformComponent *comp = n->getComponent<TransformComponent>();
        comp->setPosition(glm::vec3(0, 0, 100));
        CameraComponent *camera = n->getComponent<CameraComponent>();
        camera->setFocus(glm::vec3(0, 0, 0));
    }
    
    void Manager::loadDefaultComponent() {
        eventComponent = new EventComponent();
        addComponent(eventComponent);
        eventComponent->retain();
        
        glComponent = new GLComponent();
        addComponent(glComponent);
        glComponent->retain();
    }

    void Manager::run(const std::string& scene) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        step();
        glComponent->initGLFW();
        
        bool bLoad = false;

        time_t startTime, endTime;
        int restmilsecond = 0;

        enter();
        resume();
        running = true;
        while (running) {
            time(&startTime);

            glComponent->clearGL();
            update();
            glComponent->swap();
            
            dispatchMsg();

            AutoReleasePoolMgr::getInstance()->getCurrentPool()->clear();
            time(&endTime);
            restmilsecond = 1000/fps - difftime(endTime, startTime) * 1000;
            if (restmilsecond > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(restmilsecond));
            }

            if (!bLoad) {
                bLoad = true;
                loadScene(scene);
            }
            
        }
        suspend();
        leave();
    }

    void Manager::exit() {
        running = false;
    }
    
    bool Manager::loadScene(const std::string& scene) {
        Node* n = SceneImporter::import(scene);
        if (nullptr == n) {
            return false;
        }
        addChild(n);

        return true;
    }
    
    void Manager::sendMsg(Message *msg) {
        msg->retain();
        msgs.push_back(msg);
    }

    void Manager::sendMsg(const std::string& modName, Message* msg) {
        LOGE("Manager", "sendMsg is not support");
//        Node* child = getChild(modName);
//        if (nullptr == child) {
//            LOGE("Manager", "sendMsg can't find mod %s", modName.c_str());
//            return;
//        }
//        msg->receiver = child->getId();
//        sendMsg(msg);
    }
    
    void Manager::postEvent(int event, Object* data) {
        eventComponent->postEvent(event, data);
    }
    
    void Manager::dispatchMsg() {
        if (nullptr == eventComponent) {
            return;
        }
        eventComponent->dispatch();
    }
    
    void Manager::afterUpdate() {
        Module::afterUpdate();
        
        GLenum e = glGetError();
        if (GL_NO_ERROR != e) {
            LOGD("Manager", "OpenGL error: %x", e);
        }
    }

    void Manager::setAppName(const std::string& name) {
        appName = name;
    }
    
    void Manager::setScreenSize(int w, int h) {
        screenWidth = w;
        screenHeight = h;
    }
    
    void Manager::setBgColor(int r, int g, int b) {
        bgColorR = r;
        bgColorG = g;
        bgColorB = b;
    }
}