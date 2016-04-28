//
//  AutoReleasePool.hpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#ifndef AutoReleasePool_hpp
#define AutoReleasePool_hpp

#include <string>
#include <vector>
#include "Reference.hpp"

namespace dragon {
    class AutoReleasePool {
    public:
        AutoReleasePool();
        AutoReleasePool(const std::string& name);
        virtual ~AutoReleasePool();
        
        void addObject(Reference* ref);
        bool removeObject(Reference* ref);
        void clear();
        bool contains(const Reference* ref);
        void dump();
        
    protected:
        std::string name;
        std::vector<Reference*> objectVec;
    };
}

#endif /* AutoReleasePool_hpp */
