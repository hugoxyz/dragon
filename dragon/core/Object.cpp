//
//  Object.cpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#include "Object.hpp"
#include "UUID.hpp"

namespace dragon {
    
    Object::Object()
    : id(0)
    , name("") {
        id = UUID::newID();
    }
    
    Object::~Object() {
    }
}