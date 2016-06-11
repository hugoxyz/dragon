//
//  InputTouchEvent.hpp
//  dragon
//
//  Created by hugo on 16/6/11.
//
//

#ifndef InputTouchEvent_hpp
#define InputTouchEvent_hpp

#include "InputBaseEvent.hpp"

namespace dragon {
    class InputTouchEvent : public InputBaseEvent {
    public:
        enum class Action {
            Press,
            Move,
            Release
        };
        
        InputTouchEvent(InputType t);
        ~InputTouchEvent();

        Action getAction();
        float getX();
        float getY();
        
        void setPosition(double x, double y);
        void setAction(Action action);

    private:
        Action action;
        float posX;
        float posY;
    };
}

#endif /* InputTouchEvent_hpp */
