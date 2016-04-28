//
//  Module.hpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#ifndef Module_hpp
#define Module_hpp

#include "Node.hpp"
#include "Message.hpp"

namespace dragon {
    class Module : public Node {
    public:
        virtual bool init();
        virtual void update(int dt);
        virtual void onMessage(Message* msg);

    };
}

#endif /* Module_hpp */
