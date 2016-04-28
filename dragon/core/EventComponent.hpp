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
            EVENT_NONE,
            EVENT_ALL,
            EVENT_INPUT,
            
            EVENT_USER = 1000,
        };

        typedef void (*EventComponentCallBack)(int event, Object* params, Object* userData);

        EventComponent();
        ~EventComponent();
        
        int addObserver(int event, EventComponentCallBack cb, Object* userData = nullptr);
        void removeObserver(int event, int tag);

        void postEvent(int event, Object* params = nullptr);
        void sendEvent(int event, Object* params = nullptr);
        void dispatch();
        
        void clear();
    protected:
        struct EventData {
            EventComponentCallBack cb;
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
