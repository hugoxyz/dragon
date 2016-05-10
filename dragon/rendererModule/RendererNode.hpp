//
//  RendererNode.hpp
//  dragon
//
//  Created by hugo on 16/5/4.
//
//

#ifndef RendererNode_hpp
#define RendererNode_hpp

#include "../core/Node.hpp"

namespace dragon {
    class RendererModule;
    class RendererNode : public Node {
    public:
        friend RendererModule;
        
        void reset();

    protected:
        bool dirty;
    };
}

#endif /* RendererNode_hpp */
