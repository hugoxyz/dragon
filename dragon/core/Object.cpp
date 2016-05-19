//
//  Object.cpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#include <sstream>

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
    
    void Object::setName(const std::string &n) {
        name = n;
    }
    
    std::string Object::toString() {
        std::stringstream ss;
        ss << id << "|" << typeid(*this).name() << "|" << this;
        return ss.str();
    }
    
}