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

        virtual void onInit() {};
        virtual void onEnter() {};
        virtual void onResume() {};
        virtual void onPreUpdate() {};
        virtual void onUpdate() {};
        virtual void onAfterUpdate() {};
        virtual void onSuspend() {};
        virtual void onLeave() {};
        virtual void onDeinit() {};
        virtual bool isVisible() { return visible; };

        friend class Manager;
    protected:
        void init();
        void enter();
        void resume();
        void preUpdate();
        void update();
        void afterUpdate();
        void suspend();
        void leave();
        void deinit();

    protected:
        bool visible;
    };
}

#endif /* ILifeCycle_hpp */
