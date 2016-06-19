//
//  Layer.hpp
//  dragon
//
//  Created by hugo on 16/6/15.
//
//

#ifndef Layer_hpp
#define Layer_hpp

#include "Widget.hpp"

namespace dragon {
    namespace ui {
        class Layer : Widget {
        public:
            Layer();
            ~Layer();
            
            void setColor(int r, int g, int b, int a);
            void setSize(int w, int h);
        };
    }
}

#endif /* Layer_hpp */
