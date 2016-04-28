//
//  FBXMessage.hpp
//  dragon
//
//  Created by hugo on 16/4/19.
//
//

#ifndef FBXMessage_hpp
#define FBXMessage_hpp

#include "../core/Message.hpp"

namespace dragon {
    class FBXMessage : public Message {
    public:
        FBXMessage();
        virtual ~FBXMessage();
        
        void setFilePath(const std::string& path);
        std::string getFilePath(void);
        
    private:
        std::string filePath;
    };
}

#endif /* FBXMessage_hpp */
