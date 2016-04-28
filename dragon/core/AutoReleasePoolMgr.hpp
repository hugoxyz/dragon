//
//  AutoReleasePoolMgr.hpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#ifndef AutoReleasePoolMgr_hpp
#define AutoReleasePoolMgr_hpp

#include <vector>
#include "AutoReleasePool.hpp"

namespace dragon {
    class AutoReleasePoolMgr {
    public:
        static AutoReleasePoolMgr* getInstance();
        
        AutoReleasePool* getCurrentPool();

    protected:
        std::vector<AutoReleasePool*> poolVec;
        static AutoReleasePoolMgr* instance;
    };
}

#endif /* AutoReleasePoolMgr_hpp */
