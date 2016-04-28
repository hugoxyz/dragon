//
//  Component.cpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#include "Component.hpp"

namespace dragon {

    Component::Component()
    : host(nullptr) {
        components.clear();
    }

    Component::~Component() {
        removeAllComponent();
    }
    
    void Component::onStateEvent(StateEvent e) {
        state = e;
    }

    void Component::addComponent(const std::string& name, Component* comp) {
        auto ret = components.insert(std::make_pair(name, comp));
        if (ret.second) {
            comp->host = this;
            comp->retain();
        }
    }

    void Component::removeComponent(const std::string& name) {
        for (auto iter = components.end(); iter != components.begin(); iter++) {
            if (0 == iter->first.compare(name)) {
                iter->second->host = nullptr;
                iter->second->release();
                components.erase(iter);
            }
        }
    }
    
    void Component::removeAllComponent() {
        for (const auto& item: components) {
            item.second->host = nullptr;
            item.second->release();
        }
        components.clear();
    }
    
    template <typename T>
    T* Component::getComponent(const std::string& name) {
        auto iter = components.find(name);
        return dynamic_cast<T>(iter->second);
    }
}