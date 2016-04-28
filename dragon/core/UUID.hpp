//
//  UUID.hpp
//  dragon
//
//  Created by hugo on 16/4/15.
//
//

#ifndef UUID_hpp
#define UUID_hpp

namespace dragon {
    class UUID {
    public:
        static int newID();
        static int newSysID();
        
    private:
        static int idCounter;
        static int sysIdCounter;
        static int separateNumber;
    };
}

#endif /* UUID_hpp */
