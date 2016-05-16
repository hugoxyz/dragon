//
//  Node.hpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#ifndef Node_hpp
#define Node_hpp

#include <vector>
#include <map>
#include "Component.hpp"

namespace dragon {
    class Module;
    class Node : public Component {
    public:
        Node();
        virtual ~Node();
        
        void createTransformIf();

        /*
         * Node - child
         */
        void addChild(Node* n);
        void removeChild(int id, bool release = true);
        void removeAllChild(bool release = true);
        Node* getChild(int id);
        Node* getChild(const std::string& name);

        /*
         * Node - component
         */
        void addComponent(Component* comp, const std::string& name = "");
        void removeComponent(const std::string& name);
        void removeAllComponent();
        Component* getComponent(const std::string& name);
        template <class T>
        T* getComponent() {
            Component *comp = getComponent(typeid(T).name());
            return dynamic_cast<T*>(comp);
        }
        
        Module* getModule();

        friend class Manager;
    protected:

        /*
         * Life Cycle
         */
        virtual void enter();
        virtual void resume();
        virtual void preUpdate();
        virtual void update();
        virtual void afterUpdate();
        virtual void suspend();
        virtual void leave();

    protected:
        std::vector<Node*> children;
        std::map<std::string, Component*> components;

        Node* parent;
    };
}

#endif /* Node_hpp */
