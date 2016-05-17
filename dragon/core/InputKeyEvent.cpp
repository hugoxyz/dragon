//
//  InputKeyEvent.cpp
//  dragon
//
//  Created by hugo on 16/5/17.
//
//

#include "InputKeyEvent.hpp"

namespace dragon {
    
    InputKeyEvent::InputKeyEvent(InputType t)
    : InputBaseEvent(t)
    , inputAction(Action::Press)
    , inputKey(Key::KEY_NONE) {
    }
    
    void InputKeyEvent::setKey(Key k) {
        inputKey = k;
    }
    
    void InputKeyEvent::setAction(Action a) {
        inputAction = a;
    }
    
    InputKeyEvent::Action InputKeyEvent::getAction() {
        return inputAction;
    }
    
    InputKeyEvent::Key InputKeyEvent::getKey() {
        return inputKey;
    }

}