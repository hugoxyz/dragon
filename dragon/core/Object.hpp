//
//  Object.hpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#ifndef Object_hpp
#define Object_hpp

#include <string>
#include "Reference.hpp"

namespace dragon {
    class Node;
    class Object : public Reference {
    public:
        
        Object();
        ~Object();

        virtual int getId() { return id; };
        virtual const std::string& getName() { return name; };
        
    protected:
        int id;
        std::string name;
    };
}

#endif /* Object_hpp */
