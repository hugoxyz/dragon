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

    void Component::addComponent(Component* comp, const std::string& n) {
        std::string name;
        if (n.empty()) {
            name = typeid(*comp).name();
        } else {
            name = n;
        }
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

    Component* Component::getComponent(const std::string& name) {
        auto iter = components.find(name);
        return iter->second;
    }
    
    template <typename T>
    T* Component::getComponent() {
        Component *comp = getComponent(typeid(T).name());
        return dynamic_cast<T*>(comp);
    }
}