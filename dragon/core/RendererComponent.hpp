//
//  RendererComponent.hpp
//  dragon
//
//  Created by hugo on 16/6/17.
//
//

#ifndef RendererComponent_hpp
#define RendererComponent_hpp

#include "Component.hpp"

namespace dragon {
    class RendererComponent : public Component {
    public:
        RendererComponent();
        ~RendererComponent();
        
        virtual void onUpdate() override;
        
    };
}

#endif /* RendererComponent_hpp */
