//
//  AutoReleasePool.cpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#include "AutoReleasePool.hpp"
#include "Logger.hpp"

namespace dragon {
    AutoReleasePool::AutoReleasePool()
    : name("") {
        objectVec.clear();
    }

    AutoReleasePool::AutoReleasePool(const std::string& n)
    : name(n) {
    }

    AutoReleasePool::~AutoReleasePool() {
    }
    
    void AutoReleasePool::addObject(Reference* ref) {
        objectVec.push_back(ref);
    }

    bool AutoReleasePool::removeObject(Reference* ref) {
        for (auto item = objectVec.begin(); item !=  objectVec.end(); item ++) {
            if (*item == ref) {
                objectVec.erase(item);
                return true;
            }
        }
        
        return false;
    }

    void AutoReleasePool::clear() {
        for (auto item : objectVec) {
            item->release();
        }
        objectVec.clear();
    }
    
    bool AutoReleasePool::contains(const Reference* ref) {
        for (const auto& item : objectVec) {
            if (item == ref) {
                return true;
            }
        }
        return false;
    }
    
    void AutoReleasePool::dump() {
//        LOGD("AutoReleasePool", "pool size: %d", objectVec.size());
//        for (auto item : objectVec) {
//            LOGD("AutoReleasePool", "obj %p %d", item, item->getReferenceCount());
//        }
    }
}