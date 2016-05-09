//
//  ILifeCycle.cpp
//  dragon
//
//  Created by hugo on 16/5/7.
//
//

#include "ILifeCycle.hpp"

namespace dragon {
    
    ILifeCycle::ILifeCycle()
    : active(false) {
    }
    
    ILifeCycle::~ILifeCycle() {
    }

    void ILifeCycle::enter() {
        onEnter();
    }
    
    void ILifeCycle::resume() {
        active = true;
        onResume();
    }
    
    void ILifeCycle::preUpdate() {
        onPreUpdate();
    }
    
    void ILifeCycle::update() {
        onUpdate();
    }
    
    void ILifeCycle::afterUpdate() {
        onAfterUpdate();
    }
    
    void ILifeCycle::suspend() {
        active = false;
        onSuspend();
    }
    
    void ILifeCycle::leave() {
        onLeave();
    }

}
