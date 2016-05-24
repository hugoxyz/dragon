//
//  Importer.hpp
//  dragon
//
//  Created by hugo on 16/5/24.
//
//

#ifndef Importer_hpp
#define Importer_hpp

#include "Object.hpp"

namespace dragon {
    class Importer : public Object {
    public:
        static Node* import(const std::string& file);
    };
}

#endif /* Importer_hpp */
