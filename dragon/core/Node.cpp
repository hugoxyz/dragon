//
//  Node.cpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#include <queue>

#include "Node.hpp"
#include "Module.hpp"
#include "TransformComponent.hpp"

namespace dragon {
    
    Node::Node()
    : parent(nullptr)
    , transComp(nullptr) {
        components.clear();
    }
    
    Node::~Node() {
        removeAllComponent();
        removeAllChild();
    }
    
    void Node::createTransformIf() {
        TransformComponent *component = getComponent<TransformComponent>();
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
    
    Node* Node::findNode(const std::string& name) {
        std::queue<Node*> q;
        q.push(this);

        Node* retNode = nullptr;
        while (!q.empty()) {
            Node* n = q.front();
            q.pop();
            if (0 == n->getName().compare(name)) {
                retNode = n;
                break;
            }
            for (auto c : n->children) {
                q.push(c);
            }
        }
        
        return retNode;
    }
    
    void Node::addComponent(Component* comp, const std::string& n) {
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
        
        TransformComponent* t = dynamic_cast<TransformComponent*>(comp);
        if (t) {
            assert(nullptr == transComp);
            transComp = t;
            transComp->retain();
        }
    }
    
    void Node::removeComponent(const std::string& name) {
        for (auto iter = components.end(); iter != components.begin(); iter++) {
            if (0 == iter->first.compare(name)) {
                if (iter->second == transComp) {
                    transComp->release();
                    transComp = nullptr;
                }
                iter->second->host = nullptr;
                iter->second->release();
                components.erase(iter);
            }
        }
    }
    
    void Node::removeAllComponent() {
        for (const auto& item: components) {
            if (item.second == transComp) {
                transComp->release();
                transComp = nullptr;
            }
            item.second->host = nullptr;
            item.second->release();
        }
        components.clear();
    }
    
    Component* Node::getComponent(const std::string& name) {
        auto iter = components.find(name);
        if (iter == components.end()) {
            return nullptr;
        }
        return iter->second;
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
    
    void Node::apply(GLProgram* program) {
        for(auto comps : components) {
            comps.second->apply(program);
        }
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
    }
    
    void Node::resume() {
        Component::resume();
        for (auto child : children) {
            child->resume();
        }
        for (auto comp : components) {
            comp.second->resume();
        }
    }
    
    void Node::preUpdate() {
        if (!isActive()) {
            resume();
        }
        Component::preUpdate();
        for (auto child : children) {
            child->preUpdate();
        }
        for (auto comp : components) {
            comp.second->preUpdate();
        }
    }
    
    void Node::update() {
        Component::update();
        for (auto child : children) {
            child->update();
        }
        for (auto comp : components) {
            comp.second->update();
        }
    }
    
    void Node::afterUpdate() {
        for (auto child : children) {
            child->afterUpdate();
        }
        for (auto comp : components) {
            comp.second->afterUpdate();
        }
        Component::afterUpdate();
    }
    
    void Node::suspend() {
        Component::suspend();
        for (auto child : children) {
            child->suspend();
        }
        for (auto comp : components) {
            comp.second->suspend();
        }
    }
    
    void Node::leave() {
        Component::leave();
        for (auto child : children) {
            child->leave();
        }
        for (auto comp : components) {
            comp.second->leave();
        }
    }

}
