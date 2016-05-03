//
//  EventInput.cpp
//  dragon
//
//  Created by hugo on 16/4/29.
//
//

#include "EventInput.hpp"

namespace dragon {

    EventInput::EventInput()
    : type(EventInputType::NONE) {
        paths.clear();
    }

    EventInput::EventInput(EventInputType t) {
        paths.clear();
        type = t;
    }
    
    EventInput::~EventInput() {
        paths.clear();
    }
    
    EventInput::EventInputType EventInput::getEventType() {
        return type;
    }

    const std::vector<std::string>& EventInput::getPaths() {
        return paths;
    }
    
    void EventInput::setEventType(EventInputType t) {
        type = t;
    }
    
    void EventInput::setPaths(std::vector<std::string> paths) {
        this->paths.clear();
        this->paths = paths;
    }

}