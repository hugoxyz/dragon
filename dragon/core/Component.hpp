//
//  Component.hpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#ifndef Component_hpp
#define Component_hpp

#include "Object.hpp"
#include "ILifeCycle.hpp"

namespace dragon {

    class Node;
    class GLProgram;
    class Component : public Object, public ILifeCycle {
    public:
        Component();
        virtual ~Component();

        virtual inline void apply(GLProgram* program) {};

        friend Node;
    protected:
        Node* host;
    };
}

#endif /* Component_hpp */
