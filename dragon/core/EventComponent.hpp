//
//  EventComponent.hpp
//  dragon
//
//  Created by hugo on 16/4/28.
//
//

#ifndef EventComponent_hpp
#define EventComponent_hpp

#include <vector>
#include <map>

#include "Component.hpp"

namespace dragon {
    class EventComponent : public Component {
    public:
        enum class Event {
            EVENT_NONE = 0,
            EVENT_ALL,
            EVENT_INPUT,
            EVENT_INPUT_KEY,
            EVENT_INPUT_TOUCH,
            EVENT_RENDERER,
            EVENT_RENDERER_NODE,
            EVENT_CAMERA_PROJECTION_CHANGE,
            EVENT_CAMERA_VIEW_CHANGE,
            
            EVENT_USER = 1000,
        };
        
        typedef std::function<void(int, Object*, Object*)> EventObserverFun;

        EventComponent();
        ~EventComponent();
        
        int addObserver(int event, EventObserverFun cb, Object* userData = nullptr);
        void removeObserver(int event, int tag);

        void postEvent(int event, Object* params = nullptr);
        void sendEvent(int event, Object* params = nullptr);
        void dispatch();
        
        void clear();
    protected:
        struct EventData {
            EventObserverFun cb;
            Object* userData;
            int tag;
        };
        struct EventInfo {
            int event;
            Object* params;
        };

        std::vector<EventInfo> events;
        std::vector<EventData>* getEventData(int event);
        int increaseIndex();
        
        std::map<int, std::vector<EventData>*> _eventMap;
        int _index;
    };
}

#endif /* EventComponent_hpp */
