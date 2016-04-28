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
    : running(false)
    , eventComponent(nullptr)
    , fps(60) {
        msgs.clear();
        
        loadDefaultComponent();
        loadDefaultModule();
    }
    
    Manager::~Manager() {
        uiModule->release();
        uiModule = nullptr;
    }
    
    void Manager::loadDefaultModule() {
        Module* m = new UIModule();
        if (m->init()) {
            addChild(m);
            uiModule = m;
            uiModule->retain();
        } else {
            delete m;
            m = nullptr;
        }
        
        m = new FBXModule();
        if (m->init()) {
            addChild(m);
        } else {
            delete m;
            m = nullptr;
        }
        
        m = new RendererModule();
        if (m->init()) {
            addChild(m);
        } else {
            delete m;
            m = nullptr;
        }
    }
    
    void Manager::loadDefaultComponent() {
        auto* comp = new EventComponent();
        addComponent("__event_component", comp);
        eventComponent = comp;
    }

    void Manager::run() {
        time_t startTime, endTime;
        int restmilsecond = 0;
        
        running = true;
        while (running) {
            time(&startTime);
            
            for (const auto& child : children) {
                Module* m = dynamic_cast<Module*>(child);
                if (nullptr != m) {
                    m->update(0);
                }
            }
            dispatchMsg();
            
            AutoReleasePoolMgr::getInstance()->getCurrentPool()->clear();
            time(&endTime);
            restmilsecond = 1000/fps - difftime(endTime, startTime) * 1000;
            if (restmilsecond > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(restmilsecond));
            }
        }
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
    
    void Manager::dispatchMsg() {
        if (nullptr == eventComponent) {
            return;
        }
        eventComponent->dispatch();
    }
}