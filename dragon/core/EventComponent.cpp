//
//  EventComponent.cpp
//  dragon
//
//  Created by hugo on 16/4/28.
//
//

#include "EventComponent.hpp"

namespace dragon {
    EventComponent::EventComponent()
    : _index(0) {
        _eventMap.clear();
        events.clear();
    }
    
    EventComponent::~EventComponent() {
        clear();
    }

    int EventComponent::addObserver(int event, EventComponentCallBack cb, Object* userData) {
        auto vec = getEventData(event);
        EventData cbs;
        cbs.cb = cb;
        cbs.userData = userData;
        cbs.tag = increaseIndex();
        cbs.userData->retain();
        vec->push_back(cbs);

        return cbs.tag;
    }
    
    void EventComponent::removeObserver(int event, int tag) {
        auto vec = getEventData(event);
        auto idx = 0;
        for (auto cbs : *vec) {
            if (cbs.tag == tag) {
                cbs.userData->release();
                vec->erase(vec->begin() + idx);
                break;
            }
            idx++;
        }
    }
    
    void EventComponent::postEvent(int event, Object* params) {
        EventInfo info;
        info.event = event;
        info.params = params;
        events.push_back(info);
    }
    
    void EventComponent::sendEvent(int event, Object* params) {
        auto vec = getEventData(event);
        for (auto cbs : * vec) {
            cbs.cb(event, params, cbs.userData);
        }
    }
    
    void EventComponent::dispatch() {
        if (0 == events.size()) {
            return;
        }
        for (auto event : events) {
            sendEvent(event.event, event.params);
            event.params->release();
        }
        events.clear();
    }
    
    void EventComponent::clear() {
        for (auto it = _eventMap.end(); it != _eventMap.begin(); it--) {
            for (auto itEvent : *(it->second)) {
                itEvent.userData->release();
            }
            delete it->second;
        }
        _eventMap.clear();
    }

    std::vector<EventComponent::EventData>* EventComponent::getEventData(int event) {
        auto callbackIt = _eventMap.find(event);
        if (_eventMap.end() == callbackIt) {
            auto vec = new std::vector<EventData>();
            _eventMap[event] = vec;

            return vec;
        }
        
        return callbackIt->second;
    }
    
    int EventComponent::increaseIndex() {
        return ++_index;
    }

}