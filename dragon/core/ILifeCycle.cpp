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
    : visible(false) {
        init();
    }
    
    ILifeCycle::~ILifeCycle() {
        deinit();
    }

    void ILifeCycle::init() {
        onInit();
    }

    void ILifeCycle::enter() {
        onEnter();
    }
    
    void ILifeCycle::resume() {
        visible = true;
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
        onSuspend();
        visible = false;
    }
    
    void ILifeCycle::leave() {
        onLeave();
    }
    
    void ILifeCycle::deinit() {
        onDeinit();
    }

}
