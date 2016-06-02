//
//  RotateComponent.hpp
//  dragon
//
//  Created by hugo on 16/6/2.
//
//

#ifndef RotateComponent_hpp
#define RotateComponent_hpp

#include "Component.hpp"

namespace dragon {
    class RotateComponent : public Component {
    public:
        RotateComponent();
        ~RotateComponent();
        
        void registerKeyEvent();
        void unregisterKeyEvent();
        
        void onKeyEvent(int event, Object* data, Object* userData);
        
    protected:
        int eventKeyTag;
    };
}

#endif /* RotateComponent_hpp */
