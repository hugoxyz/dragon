//
//  Node.cpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#include "Node.hpp"
#include "Module.hpp"

namespace dragon {
    Node::Node()
    : parent(nullptr) {
    }
    
    Node::~Node() {
        removeAllChild();
    }
    
    void Node::addChild(Node* n) {
        n->parent = this;
        n->retain();
        children.push_back(n);
        n->onStateEvent(StateEvent::ENTER);
    }
    
    void Node::removeChild(int id) {
        for (auto iter = children.end(); iter != children.begin(); iter--) {
            if ((*iter)->getId() == id) {
                (*iter)->parent = nullptr;
                (*iter)->release();
                children.erase(iter);
                (*iter)->onStateEvent(StateEvent::LEAVE);
                return;
            }
        }
    }

    void Node::removeAllChild() {
        for(const auto& child : children) {
            child->parent = nullptr;
            child->release();
            child->onStateEvent(StateEvent::LEAVE);
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
    
    void Node::onStateEvent(StateEvent e) {
        for (auto child : children) {
            child->onStateEvent(e);
        }
        for (auto comp : components) {
            comp.second->onStateEvent(e);
        }
    }
}