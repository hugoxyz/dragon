//
//  InputBaseEvent.cpp
//  dragon
//
//  Created by hugo on 16/5/17.
//
//

#include "InputBaseEvent.hpp"

namespace dragon {
    
    InputBaseEvent::InputBaseEvent(InputType t)
    : inputType(t) {
    }
    
    InputBaseEvent::InputType InputBaseEvent::getInputType() {
        return inputType;
    }
    
}
