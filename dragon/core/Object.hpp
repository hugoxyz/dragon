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

#define FREEIF(p) do { if (p) { free(p); p = nullptr;} } while(0)

namespace dragon {
    class Node;
    class Object : public Reference {
    public:
        
        Object();
        ~Object();

        virtual int getId() { return id; };
        virtual const std::string& getName() { return name; };
        virtual void setName(const std::string& name);
        
    protected:
        int id;
        std::string name;
    };
}

#endif /* Object_hpp */
