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

        virtual void enter();
        virtual void resume();
        virtual void preUpdate();
        virtual void update();
        virtual void afterUpdate();
        virtual void suspend();
        virtual void leave();
        
        friend class Manager;

    protected:
        bool active;
    };
}

#endif /* ILifeCycle_hpp */
