//
//  Utils.cpp
//  dragon
//
//  Created by hugo on 16/5/10.
//
//

#include <sstream>
#include "Utils.hpp"

namespace dragon {
    
    std::vector<std::string> Utils::split(const std::string &s, char delim) {
        std::vector<std::string> elements;
        std::stringstream ss(s);
        std::string element;
        
        elements.clear();
        while (std::getline(ss, element, delim)) {
            elements.push_back(element);
        }
        
        return elements;
    }

}