//
//  Utils.hpp
//  dragon
//
//  Created by hugo on 16/5/10.
//
//

#ifndef Utils_hpp
#define Utils_hpp

#include <vector>

namespace dragon {
    
    class Utils {
        
    public:
        
        static std::vector<std::string> split(const std::string &s, char delim);
        
    };
    
}

#endif /* Utils_hpp */
