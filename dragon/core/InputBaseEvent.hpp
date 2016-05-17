//
//  InputBaseEvent.hpp
//  dragon
//
//  Created by hugo on 16/5/17.
//
//

#ifndef InputBaseEvent_hpp
#define InputBaseEvent_hpp

#include "Object.hpp"

namespace dragon {

    class InputBaseEvent : public Object {
    public:
        enum class InputType {
            Key,
            Mouse,
            Touch,
            Custom
        };
        
        InputBaseEvent(InputType t);
        InputType getInputType();

    protected:
        InputType inputType;
    };
}

#endif /* InputBaseEvent_hpp */
