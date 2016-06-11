//
//  InputTouchEvent.cpp
//  dragon
//
//  Created by hugo on 16/6/11.
//
//

#include "InputTouchEvent.hpp"

namespace dragon {
    InputTouchEvent::InputTouchEvent(InputType t)
    : InputBaseEvent(t)
    , action(InputTouchEvent::Action::Press)
    , posX(0)
    , posY(0) {
    }
    
    InputTouchEvent::~InputTouchEvent() {
    }
    
    InputTouchEvent::Action InputTouchEvent::getAction() {
        return action;
    }
    
    float InputTouchEvent::getX() {
        return posX;
    }
    
    float InputTouchEvent::getY() {
        return posY;
    }
    
    void InputTouchEvent::setPosition(double x, double y) {
        posX = x;
        posY = y;
    }
    
    void InputTouchEvent::setAction(Action a) {
        action = a;
    }
    
}