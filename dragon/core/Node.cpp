//
//  Node.cpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#include "Node.hpp"
#include "Module.hpp"
#include "TransformComponent.hpp"

namespace dragon {
    Node::Node()
    : parent(nullptr) {
    }
    
    Node::~Node() {
        removeAllChild();
    }
    
    void Node::createTransformIf() {
        TransformComponent *component = dynamic_cast<TransformComponent*>(getComponent(name));
        if (nullptr == component) {
            component = new TransformComponent();
            addComponent(component);
        }
    }
    
    void Node::addChild(Node* n) {
        n->parent = this;
        n->retain();
        children.push_back(n);
    }
    
    void Node::removeChild(int id, bool release) {
        for (auto iter = children.end(); iter != children.begin(); iter--) {
            if ((*iter)->getId() == id) {
                (*iter)->suspend();
                (*iter)->leave();
                (*iter)->parent = nullptr;
                children.erase(iter);
                (*iter)->suspend();
                if (release) {
                    (*iter)->release();
                }
                return;
            }
        }
    }

    void Node::removeAllChild(bool release) {
        for(const auto& child : children) {
            child->suspend();
            child->leave();
            child->parent = nullptr;
            if (release) {
                child->release();
            }
        }
        children.clear();
    }
    
    Node* Node::getChild(int id) {
        for (auto child : children) {
            if (child->getId() == id) {
                return child;
            }
        }
        return nullptr;
    }
    
    Node* Node::getChild(const std::string& name) {
        for (auto child : children) {
            if (0 == child->getName().compare(name)) {
                return child;
            }
        }
        return nullptr;
    }
    
    Module* Node::getModule() {
        Node* parent = this->parent;
        Node* secondParent = parent;
        
        while (parent) {
            if (parent->parent) {
                secondParent = parent;
                parent = parent->parent;
            } else {
                break;
            }
        }
        
        return dynamic_cast<Module*>(secondParent);
    }

    /*
     * Life Cycle
     */
    void Node::enter() {
        Component::enter();
        for (auto child : children) {
            child->enter();
        }
        for (auto comp : components) {
            comp.second->enter();
        }
        onEnter();
    }
    
    void Node::resume() {
        Component::resume();
        for (auto child : children) {
            child->resume();
        }
        for (auto comp : components) {
            comp.second->resume();
        }
        onResume();
    }
    
    void Node::preUpdate() {
        Component::preUpdate();
        for (auto child : children) {
            child->preUpdate();
        }
        for (auto comp : components) {
            comp.second->preUpdate();
        }
        onPreUpdate();
    }
    
    void Node::update() {
        Component::update();
        for (auto child : children) {
            child->update();
        }
        for (auto comp : components) {
            comp.second->update();
        }
        onUpdate();
    }
    
    void Node::afterUpdate() {
        Component::afterUpdate();
        for (auto child : children) {
            child->afterUpdate();
        }
        for (auto comp : components) {
            comp.second->afterUpdate();
        }
        onAfterUpdate();
    }
    
    void Node::suspend() {
        Component::suspend();
        for (auto child : children) {
            child->suspend();
        }
        for (auto comp : components) {
            comp.second->suspend();
        }
        onSuspend();
    }
    
    void Node::leave() {
        Component::leave();
        for (auto child : children) {
            child->leave();
        }
        for (auto comp : components) {
            comp.second->leave();
        }
        onLeave();
    }

}
