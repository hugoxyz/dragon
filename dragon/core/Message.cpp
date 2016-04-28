//
//  Message.cpp
//  dragon
//
//  Created by hugo on 16/4/13.
//
//

#include <stdlib.h>
#include "Message.hpp"


namespace dragon {
    Message::Message()
    : type(Type::Unknow)
    , sender(0)
    , receiver(0)
    , rawValue(nullptr)
    , rawLength(0)
    , intValue(0)
    , stringValue("") {
    }

    Message::~Message() {
        if (nullptr != rawValue) {
            free(rawValue);
            rawValue = nullptr;
            rawLength = 0;
        }
    }
    
    void Message::setValue(int i) {
        intValue = i;
        type = Type::IntValue;
    }

    void Message::setValue(const std::string& s) {
        stringValue = s;
        type = Type::StringValue;
    }

    void Message::setValue(void* p, int l) {
        if (nullptr != rawValue) {
            free(rawValue);
            rawValue = nullptr;
            rawLength = 0;
        }
        rawValue = p;
        rawLength = l;
        type = Type::RawValue;
    }

    int Message::getIntValue() {
        return intValue;
    }
    
    std::string Message::getStringValue() {
        return stringValue;
    }
    
    int Message::getRawValue(void** p) {
        *p = rawValue;
        return rawLength;
    }
    
    Message::Type Message::getType() {
        return type;
    }
    
    void Message::clearValue() {
        switch (type) {
            case Message::Type::StringValue: {
                stringValue = "";
                break;
            }
            case Message::Type::IntValue: {
                intValue = 0;
                break;
            }
            case Message::Type::RawValue: {
                if (rawValue) {
                    free(rawValue);
                    rawValue = nullptr;
                    rawLength = 0;
                }
                break;
            }
            default: {
                break;
            }
        }
        type = Message::Type::Unknow;
    }
}
