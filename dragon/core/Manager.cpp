//
//  Manager.cpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#include <unistd.h>
#include <thread>

#include "Manager.hpp"
#include "Logger.hpp"
#include "AutoReleasePoolMgr.hpp"
#include "EventComponent.hpp"
#include "CameraComponent.hpp"
#include "ControlComponent.hpp"

#include "../uiModule/UIModule.hpp"
#include "../fbxModule/FBXModule.hpp"
#include "../rendererModule/RendererModule.hpp"


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
    , fps(60) {
    }
    
    Manager::~Manager() {
        
    }
    
    void Manager::addChild(Node* n) {
        Module::addChild(n);
        n->enter();
    }

    void Manager::addComponent(Component* comp, const std::string& name) {
        Module::addComponent(comp, name);
        comp->enter();
    }
    
    void Manager::step() {
        msgs.clear();
        
        loadDefaultComponent();
        loadDefaultModule();
    }
    
    void Manager::onDeinit() {
        if (nullptr != uiModule) {
            uiModule->release();
            uiModule = nullptr;
        }
    }
    
    void Manager::loadDefaultModule() {
        Module* m = new UIModule();
        addChild(m);
        uiModule = m;
        uiModule->retain();
        
        m = new FBXModule();
        addChild(m);
        
        m = new RendererModule();
        addChild(m);
        
        Node *n = new Node();
        n->createTransformIf();
        n->addComponent(new CameraComponent());
        n->addComponent(new ControlComponent());
        n->setName("__camera_node");
        addChild(n);
    }
    
    void Manager::loadDefaultComponent() {
        auto* comp = new EventComponent();
        addComponent(comp);
        eventComponent = comp;
    }

    void Manager::run() {
        time_t startTime, endTime;
        int restmilsecond = 0;

        step();
        enter();
        resume();
        running = true;
        while (running) {
            time(&startTime);
            
            Module::preUpdate();
            Module::update();
            Module::afterUpdate();
            
            dispatchMsg();

            AutoReleasePoolMgr::getInstance()->getCurrentPool()->clear();
            time(&endTime);
            restmilsecond = 1000/fps - difftime(endTime, startTime) * 1000;
            if (restmilsecond > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(restmilsecond));
            }
        }
        suspend();
        leave();
    }

    void Manager::exit() {
        running = false;
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
}