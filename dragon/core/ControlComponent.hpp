//
//  ControlComponent.hpp
//  dragon
//
//  Created by hugo on 16/5/18.
//
//

#ifndef ControlComponent_hpp
#define ControlComponent_hpp

#include "Component.hpp"

namespace dragon {
    class ControlComponent : public Component {
    public:
        ControlComponent();
        ~ControlComponent();
        
        void registerKeyEvent();
        void unregisterKeyEvent();
        
        void onKeyEvent(int event, Object* data, Object* userData);
    };
}

#endif /* ControlComponent_hpp */
