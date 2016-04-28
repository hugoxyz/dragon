//
//  UIModule.cpp
//  dragon
//
//  Created by hugo on 16/4/13.
//
//

#include <stdlib.h>

#include "UIModule.hpp"
#include "Manager.hpp"
#include "Logger.hpp"
#include "../core/UUID.hpp"

namespace dragon {

    UIModule::UIModule() {
        name = "__ui_mod";
    }

    UIModule::~UIModule() {
    }
    
    bool UIModule::init() {
        return true;
    }
    
    void UIModule::update(int dt) {
    }

}