//
//  UIModule.hpp
//  dragon
//
//  Created by hugo on 16/4/13.
//
//

#ifndef UIModule_hpp
#define UIModule_hpp

#include "../core/Module.hpp"

namespace dragon {
    class UIModule : public Module {
    public:
        UIModule();
        ~UIModule();
        
        static UIModule* currentModule();

        virtual void update(int dt);
        
        void sendMessage(Message* m);

    };
}

#endif /* UIModule_hpp */
