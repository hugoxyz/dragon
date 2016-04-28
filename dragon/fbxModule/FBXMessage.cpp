//
//  FBXMessage.cpp
//  dragon
//
//  Created by hugo on 16/4/19.
//
//

#include "FBXMessage.hpp"

namespace dragon {
    
    FBXMessage::FBXMessage()
    : filePath("") {
    }
    
    FBXMessage::~FBXMessage() {
    }
    
    void FBXMessage::setFilePath(const std::string &path) {
        filePath = path;
    }
    
    std::string FBXMessage::getFilePath() {
        return filePath;
    }
}
