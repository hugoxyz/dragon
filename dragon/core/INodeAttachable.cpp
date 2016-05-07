//
//  NodeAttachable.cpp
//  dragon
//
//  Created by hugo on 16/5/7.
//
//

#include "INodeAttachable.hpp"
#include "Node.hpp"

namespace dragon {

    void INodeAttachable::addChild(INodeAttachable* n) {
        parent = this;
        n->retain();
        children.push_back(n);
    }
    
    void INodeAttachable::removeChild(int id) {
        for (auto iter = children.end(); iter != children.begin(); iter--) {
            if ((*iter)->getId() == id) {
                (*iter)->parent = nullptr;
                (*iter)->release();
                children.erase(iter);
                (*iter)->suspend();
                (*iter)->leave();
                return;
            }
        }
    }
    
    void INodeAttachable::removeAllChild() {
        for(const auto& child : children) {
            child->suspend();
            child->leave();
            child->parent = nullptr;
            child->release();
        }
        children.clear();
    }
    
    Node* INodeAttachable::getChild(int id) {
        for (auto child : children) {
            if (child->getId() == id) {
                return child;
            }
        }
        return nullptr;
    }
    
    Node* INodeAttachable::getChild(const std::string& name) {
        for (auto child : children) {
            if (0 == child->getName().compare(name)) {
                return child;
            }
        }
        return nullptr;
    }

    
}