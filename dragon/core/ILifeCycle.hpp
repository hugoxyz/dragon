//
//  ILifeCycle.hpp
//  dragon
//
//  Created by hugo on 16/5/7.
//
//

#ifndef ILifeCycle_hpp
#define ILifeCycle_hpp

namespace dragon {
    class Manager;
    class ILifeCycle {
    public:
        
        ILifeCycle();
        ~ILifeCycle();

        virtual void onEnter() {};
        virtual void onResume() {};
        virtual void onPreUpdate() {};
        virtual void onUpdate() {};
        virtual void onAfterUpdate() {};
        virtual void onSuspend() {};
        virtual void onLeave() {};
        virtual bool isActive() { return active; };

        friend class Manager;
    protected:

        void enter();
        void resume();
        void preUpdate();
        void update();
        void afterUpdate();
        void suspend();
        void leave();

    protected:
        bool active;
    };
}

#endif /* ILifeCycle_hpp */
