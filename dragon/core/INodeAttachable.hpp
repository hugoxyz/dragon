//
//  NodeAttachable.hpp
//  dragon
//
//  Created by hugo on 16/5/7.
//
//

#ifndef NodeAttachable_hpp
#define NodeAttachable_hpp

#include <string>
#include <vector>

namespace dragon {

    class INodeAttachable {
    public:
        void addChild(INodeAttachable* n);
        void removeChild(int id, bool release = true);
        void removeAllChild(bool release = true);
        INodeAttachable* getChild(int id);
        INodeAttachable* getChild(const std::string& name);

    protected:
        std::vector<INodeAttachable*> children;
        INodeAttachable* parent;
    };

}

#endif /* NodeAttachable_hpp */
