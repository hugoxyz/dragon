//
//  UUID.cpp
//  dragon
//
//  Created by hugo on 16/4/15.
//
//

#include "UUID.hpp"

namespace dragon {
    
    int UUID::sysIdCounter = 0;
    int UUID::separateNumber = 1000;
    int UUID::idCounter = separateNumber;
    
    int UUID::newSysID() {
        if (sysIdCounter = separateNumber) {
            return newID();
        } else {
            return ++sysIdCounter;
        }
    }

    int UUID::newID() {
        return ++idCounter;
    }
}
