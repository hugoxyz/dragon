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
#include "Component.hpp"

namespace dragon {
    class Module;
    class Node : public Component {
    public:
        Node();
        virtual ~Node();
        
        void createTransformIf();

        void onEnter();
        void onSuspend();
        void onResume();
        void onLeave();

        void addChild(Node* n);
        void removeChild(int id);
        void removeAllChild();
        Node* getChild(int id);
        Node* getChild(const std::string& name);
        
        Module* getModule();
        
    protected:
        virtual void onStateEvent(StateEvent e);

    protected:
        std::vector<Node*> children;
        Node* parent;
    };
}

#endif /* Node_hpp */
