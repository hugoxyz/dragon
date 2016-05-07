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
    enum class StateEvent {
        ENTER,
        SUSPEND,    //挂起阶段
        RESUME,     //恢复事件
        LEAVE
    };

    class Component : public Object, public ILifeCycle {
    public:
        Component();
        virtual ~Component();

        virtual void onStateEvent(StateEvent e);
        
        void addComponent(Component* comp, const std::string& name = "");
        void removeComponent(const std::string& name);
        void removeAllComponent();
        Component* getComponent(const std::string& name);
        template <typename T>
        T* getComponent();

        friend class Manager;
    protected:
        std::map<std::string, Component*> components;
        StateEvent state;
        Component* host;

    private:
        friend class Node;
    };
}

#endif /* Component_hpp */
