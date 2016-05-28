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
        static std::string normalizePath(const std::string& path);
        static std::string getFileName(const std::string& path);
        static std::string getSuffixName(const std::string& name);
        
        static int compare(const std::string& str1, const std::string& str2, bool ignoreCase = true);
    };
    
}

#endif /* Utils_hpp */
