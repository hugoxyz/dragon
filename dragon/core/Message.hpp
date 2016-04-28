//
//  Message.hpp
//  dragon
//
//  Created by hugo on 16/4/13.
//
//

#ifndef Message_hpp
#define Message_hpp

#include "Object.hpp"

namespace dragon {
    class Message : public Object {
    public:
        enum class Type {
            IntValue,
            StringValue,
            RawValue,
            Unknow,
        };
        
        /*
         * 特别注意: 在 Message 的析构函数会去释放 data 所指向中的内容，
         */
        Message();
        virtual ~Message();
        
        void setValue(int i);
        void setValue(const std::string& s);
        void setValue(void* p, int l);
        
        int getIntValue();
        std::string getStringValue();
        int getRawValue(void** p);

        Type getType();

        void clearValue();
        
        int sender;
        int receiver;
        
    protected:
        Type type;
        void* rawValue;
        int rawLength;
        int intValue;
        std::string stringValue;
    };
}

#endif /* Message_hpp */
