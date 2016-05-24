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
    
    std::string Utils::normalizePath(const std::string& path) {
        std::stringstream ss;
        
        std::string::size_type begin(0);
        std::string::size_type pos(0);
        std::string target_str = "\\\\";
        std::string replace_str = "/";
        
        do {
            pos = path.find(target_str, begin);
            if (std::string::npos == pos) {
                break;
            }
            ss << path.substr(begin, pos);
            ss << replace_str;
        } while (true);
        ss << path.substr(begin);

        return ss.str();
    }

    std::string Utils::getFileName(const std::string& path) {
        std::string filePath = normalizePath(path);
        std::string::size_type slash_pos = path.find_last_of('/');
        
        if (std::string::npos == slash_pos) {
            return path;
        } else {
            return filePath.substr(slash_pos + 1);
        }
    }

    std::string Utils::getSuffixName(const std::string& name) {
        std::string fileName = getFileName(name);
        std::string::size_type point_pos = fileName.find_last_of('.');
        
        if (point_pos == std::string::npos) {
            // not find point
            return "";
        } else {
            return fileName.substr(point_pos + 1);
        }
    }

}