//
//  Reference.cpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#include "Reference.hpp"
#include "AutoReleasePoolMgr.hpp"

namespace dragon {
    Reference::Reference()
    : referenceCount(1) {
        AutoReleasePoolMgr::getInstance()->getCurrentPool()->addObject(this);
    }
    
    Reference::~Reference() {
    }
    
    void Reference::retain() {
        referenceCount++;
    }
    
    void Reference::release() {
        referenceCount--;
        if (0 == referenceCount) {
            delete this;
        }
    }
    
    unsigned int Reference::getReferenceCount() const {
        return referenceCount;
    }
}