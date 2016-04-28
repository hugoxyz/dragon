//
//  AutoReleasePoolMgr.cpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#include "AutoReleasePoolMgr.hpp"

namespace dragon {
    
    AutoReleasePoolMgr* AutoReleasePoolMgr::instance = nullptr;
    AutoReleasePoolMgr* AutoReleasePoolMgr::getInstance() {
        if (nullptr == instance) {
            instance = new AutoReleasePoolMgr();
            auto pool = new AutoReleasePool("default");
            instance->poolVec.push_back(pool);
        }
        
        return instance;
    }
    
    AutoReleasePool* AutoReleasePoolMgr::getCurrentPool() {
        return poolVec.back();
    }

}
