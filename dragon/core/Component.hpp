//
//  Component.hpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#ifndef Component_hpp
#define Component_hpp

#include <string>
#include <map>
#include "Object.hpp"
#include "ILifeCycle.hpp"

namespace dragon {
    
    class Component : public Object, public ILifeCycle {
    public:
        Component();
        virtual ~Component();
        
        void addComponent(Component* comp, const std::string& name = "");
        void removeComponent(const std::string& name);
        void removeAllComponent();
        Component* getComponent(const std::string& name);
        template <class T>
        T* getComponent() {
            Component *comp = getComponent(typeid(T).name());
            return dynamic_cast<T*>(comp);
        }

        friend class Manager;
    protected:
        std::map<std::string, Component*> components;
        Component* host;

    private:
        friend class Node;
    };
}

#endif /* Component_hpp */
