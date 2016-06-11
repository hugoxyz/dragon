//
//  Widget.hpp
//  dragon
//
//  Created by hugo on 16/6/11.
//
//

#ifndef Widget_hpp
#define Widget_hpp

#include "../core/Node.hpp"
#include "../core/InputKeyEvent.hpp"
#include "../core/InputTouchEvent.hpp"
#include "glm.hpp"

namespace dragon {
    namespace ui {
        class Widget : public Node {
        public:
            Widget();
            virtual ~Widget();

            virtual void loadComponent();

            virtual bool isVisible();
            virtual void setVisible(bool visible);

        protected:
            
            void setSwallowed(bool swallowed);
            
            bool dispatchEvent(InputBaseEvent *e);
            bool dispatchTouchEvent(InputTouchEvent *e);
            bool dispatchKeyEvent(InputKeyEvent *e);

            // touch event
            bool onTouchBegin(InputTouchEvent *e);
            bool onTouchMove(InputTouchEvent *e);
            bool onTouchEnd(InputTouchEvent *e);
            
            // key event
            bool onKeyBegin(InputKeyEvent *e);
            bool onKeyRepeat(InputKeyEvent *e);
            bool onKeyEnd(InputKeyEvent *e);
            
        protected:
            bool visible;
            
        private:
            void registerTouchEvent();
            void unregisterTouchEvent();
            void onTouchEvent(int event, Object* data, Object* userData);
            void registerKeyEvent();
            void unregisterKeyEvent();
            void onKeyEvent(int event, Object* data, Object* userData);
            
        private:
            int tagTouchEvent;
            int tagKeyEvent;
            
            bool swallowed;
        };
    }
}

#endif /* Widget_hpp */
