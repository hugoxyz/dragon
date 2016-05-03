//
//  EventInput.hpp
//  dragon
//
//  Created by hugo on 16/4/29.
//
//

#ifndef EventInput_hpp
#define EventInput_hpp

#include <vector>
#include "Object.hpp"

namespace dragon {
    class EventInput : public Object {
    public:
        enum class EventInputType {
            NONE,
            DROP_FILE,
        };
        
        EventInput();
        EventInput(EventInputType t);
        ~EventInput();
        
        EventInputType getEventType();
        const std::vector<std::string>& getPaths();

        void setEventType(EventInputType t);
        void setPaths(std::vector<std::string> paths);

    protected:
        EventInputType type;
        std::vector<std::string> paths;
    };
}

#endif /* EventInput_hpp */
