//
//  RectComponent.hpp
//  dragon
//
//  Created by hugo on 16/6/14.
//
//

#ifndef RectComponent_hpp
#define RectComponent_hpp

#include "glm.hpp"

#include "Component.hpp"
#include "GLProgram.hpp"

namespace dragon {
    class RectComponent : public Component {
    public:
        RectComponent();
        ~RectComponent();
        
        virtual void onUpdate() override;
        
        void setFillColor(const glm::vec4& color);
        void setBorderColor(const glm::vec4& color);
        void setBorderWidth(int w);
        
    protected:
        glm::vec4 fillColor;
        glm::vec4 borderColor;
        int borderWidth;
        
        GLProgram* glProgram;
        Node* camera;
    };
}

#endif /* RectComponent_hpp */
